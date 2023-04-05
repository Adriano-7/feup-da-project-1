#ifndef PROJECT_DA_DATABASE_H
#define PROJECT_DA_DATABASE_H

#include "Graph.h"
#include <unordered_map>

class Database {
    public:
        Database() = default;
        void loadWithoutFilters();
        void loadWithFilters(set<string> stations, set<string> lines);
        Station& getStation(int id);
        vector<string> getDistricts();
        vector<string> getMunicipalities(string district);
        vector<int> getStations(string municipality);
        int getNumNodes();


        vector<pair<Node *, Node *>> maxFlowAllPairs(int *maxFlow);
        int getMaxFlowBetweenStations(int station1, int station2);
        vector<pair<string, int>> getTopMunicipies(int k);
        vector<pair<string, int>>  getTopDistricts(int k);
        int getMaxTrainsStation(int id);
        vector<Node*> getMinCostFlow(int station1, int station2, double *flow, double *cost);


private:
        Graph graph;
        unordered_map<string, Station*> nameToStation;
        map<string, vector<int>> stationsByMunicipality;
        map<string, vector<string>> municipalitiesByDistrict;
        vector<string> districts;
        void sortDistricts();
        void sortMunicipalities();

        void readStations(set<string> stations, set<string> lines);
        void readNetwork();
};

#endif //PROJECT_DA_DATABASE_H
