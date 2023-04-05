#include "../headers/Station.h"

/**
 * @brief Class constructor of Station that sets name, district, municipality and line
 * @details Time complexity: O(1)
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
 */
string Station::getName() const {
    return name;
}

/**
 * @brief Returns the district of the station
 * @details Time complexity: O(1)
 */
string Station::getDistrict() const{
    return district;
}

/**
 * @brief Returns the municipality of the station
 * @details Time complexity: O(1)
 */
string Station::getMunicipality() const{
    return municipality;
}

/**
 * @brief Returns the township of the station
 * @details Time complexity: O(1)
 */
string Station::getTownship() const{
    return township;
}

/**
 * @brief Returns the line of the station
 * @details Time complexity: O(1)
 */
string Station::getLine() const{
    return line;
}