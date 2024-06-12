"""
@file: fetchAirportData.py
@author: 0Ykahil

Script to grab airport location data from OurAirports API
and convert it into a JSON file
"""
import os
import json
from ourairports import OurAirports



# Create an instance of the OurAirports API
our_airports = OurAirports()

def filter_by_region(airports, region: str) -> list:
    """_summary_

    Args:
        airports (list[dict]): List of airport dictionary data containing location 
                               info and more.
        region (str): Region of interest (one of: NA, SA, EU, AF, AS)

    Returns:
        Filtered list of airports in the given region
    """
    filtered = [] # filtered contains the airports matching our types
    
    for airport in airports:
        if airport['continent'] == region:
            filtered.append(airport)

    return filtered


def parse_airport_data(airports: list):
    """_summary_

    Args:
        airports (list): list of Airport Objects with all the information.

    Returns:
        _type_: parsed list with elements of interest 
                (ident, name, type, long, lat, continent, iata, icao)
    """
    airports_data = []  # contains the parsed airport objects

    for airport in airports:
        airport_data = {
            'ident': airport.ident,
            'name': airport.name,
            'type': airport.type,
            'latitude': airport.latitude,
            'longitude': airport.longitude,
#            'elevation': airport.elevation,
            'continent': airport.continent,
#            'country': airport.country,
            'iata': airport.iata,
            'icao': airport.icao
        }
        airports_data.append(airport_data)
        
    return airports_data
        
        
        
def main():
    # fetch the airports by the desired type (medium and large airports have consistent ids)
    try:
        med_ap = our_airports.getAirportsByType('medium_airport')
        lg_ap = our_airports.getAirportsByType('large_airport')
    except:
        print("Exception occured while fetching airports by type")
    
    # parse the data from objects to JSON compatible dict objects and combine the lists
    airports_data = parse_airport_data(med_ap) + parse_airport_data(lg_ap)

    # filter for north america and some central america
    ap_filt = filter_by_region(airports_data, "NA")

    # Write the data to a JSON file
    output_dir = './datasets'
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, 'global_airports.json')
    try: 
        with open(output_path, 'w') as file:
            json.dump(airports_data, file, indent=4)
        print('Data written to global_airports.json')
    except:
        print("Exception occured while writing JSON file to directory")


if __name__ == "__main__":
    main()