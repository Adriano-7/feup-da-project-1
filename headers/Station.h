#ifndef PROJECT_DA_STATION_H
#define PROJECT_DA_STATION_H

#include <string>

using namespace std;

class Station {
    public:
    // Constructor
        Station(int id,string name, string district, string municipality, string township, string line);
    // Getters
        int getId() const;
        string getName() const;
        string getDistrict() const;
        string getMunicipality() const;
        string getTownship() const;
        string getLine() const;
    private:
        int id;
        string name;
        string district;
        string municipality;
        string township;
        string line;
};


#endif //PROJECT_DA_STATION_H
