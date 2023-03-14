#ifndef PROJECT_DA_STATION_H
#define PROJECT_DA_STATION_H

#include <string>

using namespace std;

class station {
    public:

    // Constructor
        station(string name, string district, string municipality, string township, string line);

    // Getters
        string getName();
        string getDistrict();
        string getMunicipality();
        string getTownship();
        string getLine();

    private:
        string name;
        string district;
        string municipality;
        string township;
        string line;
};


#endif //PROJECT_DA_STATION_H
