#ifndef PROJECT_DA_DATABASE_H
#define PROJECT_DA_DATABASE_H

#include "Graph.h"
#include <unordered_map>

class Database {
    public:
        Database() = default;
        void printNodes();
        void printEdges();
        void loadWithoutFilters();
        void loadWithFilters(set<string> stations, set<string> lines);
        Station& getStation(int id);
        vector<string> getDistricts();
        vector<string> getMunicipalities(string district);
        vector<int> getStations(string municipality);


        vector<pair<Node *, Node *>> maxFlowAllPairs(int *maxFlow);

        int getMaxFlowBetweenStations(int station1, int station2);
        int getMaxTrainsStation(string station);


private:
        Graph graph;
        unordered_map<string, Station*> nameToStation;
        map<string, vector<int>> stationsByMunicipality; //TODO
        map<string, vector<string>> municipalitiesByDistrict; //TODO
        vector<string> districts; //TODO
        void sortDistricts();
        void sortMunicipalities();

        void readStations(set<string> stations, set<string> lines);
        void readNetwork();

};


#endif //PROJECT_DA_DATABASE_H
