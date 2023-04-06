#include "../headers/Station.h"

/**
 * @brief Class constructor of Station that sets name, district, municipality and line
 * @details Time complexity: O(1)
 * @param name - Name of the station
 * @param district - District of the station
 * @param municipality - Municipality of the station
 * @param township - Township of the station
 * @param line - Line of the station
 */
Station::Station(string name, string district, string municipality, string township, string line) {
    this->name = name;
    this->district = district;
    this->municipality = municipality;
    this->township = township;
    this->line = line;
}

/**
 * @brief Returns the name of the station
 * @details Time complexity: O(1)
 * @return Name of the station
 */
string Station::getName() const {
    return name;
}

/**
 * @brief Returns the district of the station
 * @details Time complexity: O(1)
 * @return District of the station
 */
string Station::getDistrict() const{
    return district;
}

/**
 * @brief Returns the municipality of the station
 * @details Time complexity: O(1)
 * @return Municipality of the station
 */
string Station::getMunicipality() const{
    return municipality;
}

/**
 * @brief Returns the township of the station
 * @details Time complexity: O(1)
 * @return Township of the station
 */
string Station::getTownship() const{
    return township;
}

/**
 * @brief Returns the line of the station
 * @details Time complexity: O(1)
 * @return Line of the station
 */
string Station::getLine() const{
    return line;
}