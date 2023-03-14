#include "station.h"

station::station(string name, string district, string municipality, string township, string line) {
    this->name = name;
    this->district = district;
    this->municipality = municipality;
    this->township = township;
    this->line = line;
}

string station::getName() {
    return name;
}

string station::getDistrict() {
    return district;
}

string station::getMunicipality() {
    return municipality;
}

string station::getTownship() {
    return township;
}

string station::getLine() {
    return line;
}
