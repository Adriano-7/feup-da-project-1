#ifndef PROJECT_DA_STATION_H
#define PROJECT_DA_STATION_H

#include <string>

using namespace std;

class Station {
    public:
    // Constructor
        Station(string name, string district, string municipality, string township, string line);
    // Getters
        string getName() const;
        string getDistrict() const;
        string getMunicipality() const;
        string getTownship() const;
        string getLine() const;
    private:
        string name;
        string district;
        string municipality;
        string township;
        string line;
};


#endif //PROJECT_DA_STATION_H
