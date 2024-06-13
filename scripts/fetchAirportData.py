import os
import json
import requests
import csv
from io import StringIO

def fetch_airport_data():
    """
    Fetches the airport data from the ourairports database csv file 
    or raises Exception if failed
    """
    url = "https://davidmegginson.github.io/ourairports-data/airports.csv"  # URL of airport data
    response = requests.get(url)
    response.encoding = 'utf-8'  # To stop breaking of fetch script on windows compiler
    if response.status_code != 200:
        raise Exception(f"Failed to fetch data: {response.status_code}")
    return response.text



def parse_csv_data(csv_data):
    """
    Parses the csv data to dictionary object to be parsed into json
    """
    airports = []
    reader = csv.DictReader(StringIO(csv_data))
    for row in reader:
        airports.append(row)
    return airports

def filter_by_region(airports, region: str) -> list:
    """Filter airports by region.

    Args:
        airports (list[dict]): List of airport dictionary data containing location 
                               info and more.
        region (str): Region of interest (one of: NA, SA, EU, AF, AS)

    Returns:
        list: Filtered list of airports in the given region
    """
    filtered = []  # filtered contains the airports matching our types
    
    for airport in airports:
        if airport['continent'] == region:
            filtered.append(airport)

    return filtered

def parse_airport_data(airports: list):
    """Parse airport data into JSON format.

    Args:
        airports (list[dict]): List of Airport objects with all the information.

    Returns:
        list: Parsed list with elements of interest 
              (ident, name, type, latitude, longitude, continent, iata, icao)
    """
    airports_data = []  # contains the parsed airport objects

    for airport in airports:
        airport_data = {
            'ident': airport['ident'],
            'name': airport['name'],
            'type': airport['type'],
            'latitude': airport['latitude_deg'],
            'longitude': airport['longitude_deg'],
            'continent': airport['continent'],
            'iata': airport['iata_code'],
            'icao': airport['gps_code']  # corrected key based on available data
        }
        airports_data.append(airport_data)
        
    return airports_data

def main():
    try:
        csv_data = fetch_airport_data()
    except Exception as e:
        print(f"Exception occurred while fetching data: {e}")
        return

    try:
        raw_airports = parse_csv_data(csv_data)
    except Exception as e:
        print(f"Exception occurred while parsing CSV data: {e}")
        return

    # Filter for medium and large airports
    airports_data = [airport for airport in raw_airports if airport['type'] in ['medium_airport', 'large_airport']]

    # Filter for North America and some Central America
    ap_filt = filter_by_region(airports_data, "NA")

    # Parse the filtered data into JSON-compatible format
    parsed_airports_data = parse_airport_data(ap_filt)

    # Write the data to a JSON file
    output_dir = './datasets'
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, 'airports.json')
    try: 
        with open(output_path, 'w', encoding='utf-8') as file:
            json.dump(parsed_airports_data, file, indent=4)
        print('Data written to airports.json')
    except Exception as e:
        print(f"Exception occurred while writing JSON file to directory: {e}")

if __name__ == "__main__":
    main()
