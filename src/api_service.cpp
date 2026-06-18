#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <csignal>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include "utility_functions.h"
#include "Graph.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int PORT = 8080;
std::atomic<int> aircraftRangeNm(500);
utility::string_t web_link = "http://0.0.0.0:" + std::to_string(PORT);

nlohmann::json airportsData;
std::mutex graphCacheMutex;
std::unordered_map<int, std::shared_ptr<Graph>> graphCache;
std::atomic<bool> running(true);

void handleShutdownSignal(int) {
    running = false;
}

void addCorsHeaders(http_headers& headers) {
    headers.add(U("Access-Control-Allow-Origin"), U("*"));
    headers.add(U("Access-Control-Allow-Methods"), U("GET, PUT, OPTIONS"));
    headers.add(U("Access-Control-Allow-Headers"), U("Content-Type"));
}

void sendJson(http_request request, status_code status, const json::value& body) {
    http_response response(status);
    response.headers().set_content_type(U("application/json"));
    addCorsHeaders(response.headers());
    response.set_body(body);
    request.reply(response);
}

void handleOptions(http_request request) {
    http_response response(status_codes::OK);
    addCorsHeaders(response.headers());
    request.reply(response);
}

std::shared_ptr<Graph> getGraphForRange(int rangeNm) {
    {
        std::lock_guard<std::mutex> lock(graphCacheMutex);
        auto it = graphCache.find(rangeNm);
        if (it != graphCache.end()) {
            return it->second;
        }
    }

    std::shared_ptr<Graph> graph = std::make_shared<Graph>(airportsData.size());
    graph->generateAirportGraph(airportsData, rangeNm, false);

    std::lock_guard<std::mutex> lock(graphCacheMutex);
    auto [it, inserted] = graphCache.insert({rangeNm, graph});
    return it->second;
}

/**
 * Handles GET /health.
 * Returns a simple JSON response to confirm the API server is running.
 */
void handleHealth(http_request request) {
    json::value response;
    response[U("status")] = json::value::string(U("ok"));

    sendJson(request, status_codes::OK, response);
}

/**
 * Handles GET /config.
 * Returns the current API configuration values.
 */
void handleGetConfig(http_request request) {
    json::value response;
    response[U("aircraftRangeNm")] = json::value::number(aircraftRangeNm.load());

    sendJson(request, status_codes::OK, response);
}


/**
 * Handles PUT /config/range?range=<rangeInNm>.
 * Updates the default aircraftRangeNm for route requests that do not provide a range.
 */
void handleSetRange(http_request request) {
    utility::string_t queryString = request.request_uri().query();
    std::map<utility::string_t, utility::string_t> queryParams = uri::split_query(queryString);
    auto rangeParam = queryParams.find(U("range"));

    json::value response;
    if (rangeParam != queryParams.end()) {
        utility::string_t range = rangeParam->second;
        try {
            int rangeInt = std::stoi(utility::conversions::to_utf8string(range));
            if (rangeInt <= 0) {
                response[U("error")] = json::value::string(U("range must be greater than 0"));
                sendJson(request, status_codes::BadRequest, response);
                return;
            }

            aircraftRangeNm.store(rangeInt);
            response[U("aircraftRangeNm")] = json::value::number(aircraftRangeNm.load());
            sendJson(request, status_codes::OK, response);
        } catch (const std::exception&) {
            response[U("error")] = json::value::string(U("invalid range value"));
            sendJson(request, status_codes::BadRequest, response);
        }
    } else {
        response[U("error")] = json::value::string(U("missing range parameter"));
        sendJson(request, status_codes::BadRequest, response);
    }
}

/**
 * Handles GET /airports?search=<query>.
 * Searches airport identifiers and names for the provided query string.
 */
void handleAirportSearch(http_request request) {
    utility::string_t queryString = request.request_uri().query();
    std::map<utility::string_t, utility::string_t> queryParams = uri::split_query(queryString);
    auto searchParam = queryParams.find(U("search"));

    json::value response;

    if (searchParam != queryParams.end()) {
        utility::string_t search = searchParam->second;
        std::string searchText = toUpperCase(utility::conversions::to_utf8string(search));

        response[U("search")] = json::value::string(search);
        response[U("airports")] = json::value::array();
    
        int index = 0;
        // index all results
        for (const nlohmann::json& airport : airportsData) {
            std::string code = toUpperCase(airport["ident"]);
            std::string name = toUpperCase(airport["name"]);
            std::string type = airport["type"];

            bool matchesCode = code.find(searchText) != std::string::npos;
            bool matchesName = name.find(searchText) != std::string::npos;

            if (matchesCode || matchesName) {
                json::value airportJson;

                airportJson[U("code")] = json::value::string(utility::conversions::to_string_t(code));
                airportJson[U("name")] = json::value::string(utility::conversions::to_string_t(name));
                airportJson[U("type")] = json::value::string(utility::conversions::to_string_t(type));

                response[U("airports")][index] = airportJson;
                index++;
            }
        }

    } else {
        response[U("error")] = json::value::string(U("missing search parameter"));
        sendJson(request, status_codes::BadRequest, response);
        return;
    }

    sendJson(request, status_codes::OK, response);
}

/**
 * Handles GET /airports/{code}.
 * Returns detailed information about the airport with the specified code.
 */
void handleAirportDetail(http_request request) {
    utility::string_t path = request.relative_uri().path();
    utility::string_t code = path.substr(path.find_last_of(U("/")) + 1);
    json::value response;

    for (const nlohmann::json& airport : airportsData) {
        if (toUpperCase(airport["ident"]) == toUpperCase(utility::conversions::to_utf8string(code))) {
            response[U("code")] = json::value::string(utility::conversions::to_string_t((std::string) airport["ident"]));
            response[U("name")] = json::value::string(utility::conversions::to_string_t((std::string) airport["name"]));
            response[U("type")] = json::value::string(utility::conversions::to_string_t((std::string) airport["type"]));
            response[U("latitude")] = json::value::string(utility::conversions::to_string_t((std::string) airport["latitude"]));
            response[U("longitude")] = json::value::string(utility::conversions::to_string_t((std::string) airport["longitude"]));
            response[U("continent")] = json::value::string(utility::conversions::to_string_t((std::string) airport["continent"]));

            sendJson(request, status_codes::OK, response);
            return;
        }
    }
    
    response[U("message")] = json::value::string(U(code + " Not found in dataset"));

    sendJson(request, status_codes::NotFound, response);
}

/**
 * Handles GET /route?start=<startCode>&dest=<destCode>&range=<desiredRange>
 */
void handleRoute(http_request request) {
    json::value response;

    utility::string_t queryString = request.request_uri().query();
    std::map<utility::string_t, utility::string_t> queryParams = uri::split_query(queryString);

    auto startParam = queryParams.find(U("start"));
    auto destParam = queryParams.find(U("dest"));
    auto modeParam = queryParams.find(U("mode"));
    auto rangeParam = queryParams.find(U("range"));
    std::string startCode = startParam != queryParams.end() ? utility::conversions::to_utf8string(startParam->second) : "";
    std::string destCode = destParam != queryParams.end() ? utility::conversions::to_utf8string(destParam->second) : "";
    int mode = 0;
    int routeRangeNm = aircraftRangeNm.load();

    if (modeParam != queryParams.end()) {
        try {
            mode = std::stoi(utility::conversions::to_utf8string(modeParam->second));
        } catch (const std::exception&) {
            response[U("error")] = json::value::string(U("invalid mode parameter"));
            sendJson(request, status_codes::BadRequest, response);
            return;
        }
    }

    if (rangeParam != queryParams.end()) {
        try {
            routeRangeNm = std::stoi(utility::conversions::to_utf8string(rangeParam->second));
            if (routeRangeNm <= 0) {
                response[U("error")] = json::value::string(U("range must be greater than 0"));
                sendJson(request, status_codes::BadRequest, response);
                return;
            }
        } catch (const std::exception&) {
            response[U("error")] = json::value::string(U("invalid range parameter"));
            sendJson(request, status_codes::BadRequest, response);
            return;
        }
    }

    std::shared_ptr<Graph> routeGraph = getGraphForRange(routeRangeNm);

    if (startCode.empty() || !routeGraph->isValidAirport(startCode)) {
        response[U("error")] = json::value::string(U("invalid or missing start parameter"));
        sendJson(request, status_codes::BadRequest, response);
        return;
    }

    if (destCode.empty() || !routeGraph->isValidAirport(destCode)) {
        response[U("error")] = json::value::string(U("invalid or missing destination parameter"));
        sendJson(request, status_codes::BadRequest, response);
        return;
    }

    std::pair<std::vector<std::string>, double> res = routeGraph->getShortestPath(startCode, destCode, mode);

    if (res.first.empty()) {
        response[U("error")] = json::value::string(U("no reachable path found"));
        sendJson(request, status_codes::NotFound, response);
        return;
    }

    response[U("start")] = json::value::string(utility::conversions::to_string_t(startCode));
    response[U("dest")] = json::value::string(utility::conversions::to_string_t(destCode));
    response[U("distance")] = json::value::number(res.second);
    response[U("path")] = json::value::array(res.first.size());
    response[U("rangeNm")] = json::value::number(routeRangeNm);

    for (size_t i = 0; i < res.first.size(); i++) {
        response[U("path")][i] = json::value::string(utility::conversions::to_string_t(res.first[i]));
    }

    sendJson(request, status_codes::OK, response);
}

void handleGet(http_request request) {
    utility::string_t path = request.relative_uri().path();

    if (path == U("/health")) {
        handleHealth(request);
    }
    else if (path == U("/airports")) {
        handleAirportSearch(request);
    }
    else if (path == U("/config")) {
        handleGetConfig(request);
    }
    else if (path.rfind(U("/airports/"), 0) == 0) {
        handleAirportDetail(request);
    }
    else if (path == U("/route")) {
        handleRoute(request);
    }
    else {
        json::value response;
        response[U("error")] = json::value::string(U("endpoint not found"));

        sendJson(request, status_codes::NotFound, response);
    }
}

void handlePut(http_request request) {
    utility::string_t path = request.relative_uri().path();

    if (path == U("/config/range")) {
            handleSetRange(request);
    }
    else {
        json::value response;
        response[U("error")] = json::value::string(U("endpoint not found"));
        sendJson(request, status_codes::NotFound, response);
    }
}


int main() {
    std::signal(SIGINT, handleShutdownSignal);
    std::signal(SIGTERM, handleShutdownSignal);

    http_listener listener(web_link);


    std::ifstream file("./datasets/airports.json");

    // read airports data from airports.json
    try {
        if (!file.is_open()) {
            std::cerr << "Error: Could not open airports dataset" << std::endl;
            return 1;
        }
        file >> airportsData;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse airports dataset: " << e.what() << std::endl;
        return 1;
    }

    getGraphForRange(aircraftRangeNm.load());

    listener.support(methods::GET, handleGet);
    listener.support(methods::PUT, handlePut);
    listener.support(methods::OPTIONS, handleOptions);
    try {
        listener.open().wait();
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to start API listener: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "API running at http://localhost:" << PORT << std::endl;
    std::cout << "Available endpoints:" << std::endl;
    std::cout << "  GET /health" << std::endl;
    std::cout << "  GET /airports?search=ottawa" << std::endl;
    std::cout << "  GET /airports/CYOW" << std::endl;
    std::cout << "  GET /config" << std::endl;
    std::cout << "  PUT /config/range?range=500" << std::endl;
    std::cout << "  GET /route?start=CYOW&dest=CYYZ" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    try {
        listener.close().wait();
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to close API listener: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
