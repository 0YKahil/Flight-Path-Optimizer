/**
 * @file: Airport.cpp
 * @author: 0Ykahil
 * 
 * Implementation of Airport Object
 */

#include "Airport.h"


// Earth's radius in nm
const double EARTH_RADIUS_NM = 3440.065;

// Convert deg to rad
double toRadians(double deg) {
    return deg * M_PI / 180;
    }

// Haversine Formula
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1); // diff in latitude
    double dLon = toRadians(lon2 - lon1); // diff in longitude

    // calculating inside the brackets
    double inside = sin(dLat/2)*sin(dLat/2) + cos(toRadians(lat1))*cos(toRadians(lat2)) *
                    sin(dLon/2)*sin(dLon/2);

    return EARTH_RADIUS_NM*2*atan2(sqrt(inside), sqrt(1 - inside));
}

Airport::Airport(const std::string& id, const std::string& name, const std::string& t, 
                double lat, double lon)
                : id(id), name(name), type(t), latitude(lat), longitude(lon) {}

Airport::Airport() : id("CODE"), name("NAME"), type("TYPE"), latitude(-1), longitude(-1) {}


double Airport::distanceTo(const Airport& other) const {
    return haversine(latitude, longitude, other.latitude, other.longitude);
}

bool Airport::operator==(const Airport& other) const {
    return this->id == other.id;
}