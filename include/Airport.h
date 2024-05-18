/**
 * @file: Airport.h
 * @author: 0Ykahil
 * 
 * Declaration of Airport object which will be used to store airport
 * objects after parsing airports.json
 */
#pragma once

#include <string>
#include <cmath>

// Converts degree coordinates to radians for formula use
double toRadians(double degrees);

/**
 * Calculates the distance between two locations, given their longitudinal and latitudinal degree coordinates, 
 * using the haversine equation.
 * 
 * @param lat1 The latitude of the first location.
 * @param lon1 The longitude of the first location.
 * @param lat2 The latitude of the second location.
 * @param lon2 The longitude of the second location.
 * @return The distance between the two locations in nautical miles.
 */
double haversine(double lat1, double lon1, double lat2, double lon2);

/**
 * @class Airport
 * Represents an airport with its identifier, name, type, id, latitude, and longitude.
 */
class Airport {
    public:
        std::string id;     // Identifier of airport by priority if available (IATA > ICAO > Ident).
        std::string name;   // Airport name.
        std::string type;   // Airport type (one of: small_airport, medium_airport, large_airport).
        double latitude;    // Latitude of airport.
        double longitude;   // Longitude of airport.

        /**
         * Constructor for Airport.
         * 
         * @param id The airport id/identifier.
         * @param name The name of the airport.
         * @param type The type of the airport (one of small_airport, medium_airport, or large_airport).
         * @param lat  The latitude of the airport (in degrees).
         * @param lon  The longitude of the airport (in degrees).
         */
        Airport(const std::string& id, const std::string& name, const std::string& type, 
                double lat, double lon);
        
        /**
         * Empty constructor for Airport with no values.
         */
        Airport();

        /**
         * Calculates the great circle distance (orthodromic distance) between this airport and another airport using the haversine formula.
         * 
         * @param other The airport to which the distance is being calculated.
         * @return The distance from this airport to the other airport in nautical miles.
         */
        double distanceTo(const Airport& other) const;  

        /**
         * Compares airports based on their identifying id.
         * 
         * @param other The other airport to compare with.
         * @return true if the airports have the same identifying id; false otherwise.
         */
        bool operator==(const Airport& other) const;

};


