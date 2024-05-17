/*
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
 * Calculates the distance between two locations given their longitudal and latitudal
 * degree coordinates using the haversine equation (link in Readme)
 */
double haversine(double lat1, double lon1, double lat2, double lon2);

class Airport {
    public:
        std::string id;     // identifier of airport by priority if available (IATA > ICAO > Ident)
        std::string name;   // Airport name
        std::string type;   // Airport type (one of: small_airport, medium_airport, large_airport)
        std::string code;   // Airport Code/Identifier depending on availability
        double latitude;    // Latitude of airport
        double longitude;   // Longitude of airport

        // Constructor for Airport
        Airport(const std::string& code, const std::string& name, const std::string& t, 
                double lat, double lon);
        
        // Empty constructor with no values
        Airport();

        /*
         * Calculates the great circle distance (aka orthodromic distance) 
         * between this airport and other airport using the haversine formula
         * @params: other - the airport who's distance is being compared to
         * 
         * @return: the distance from this airport to other airport in nautical miles
         */
        double distanceTo(const Airport& other) const;  

        // Compare airports by their identifying code
        bool operator==(const Airport& other) const;
        

};


