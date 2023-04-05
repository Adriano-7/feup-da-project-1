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
        Station* getStation(string stationName);
        vector<pair<Node *, Node *>> maxFlowAllPairs(int *maxFlow);

        int getMaxFlowBetweenStations(string station1, string station2);
        vector<Node*> getMinCostFlow(string station1, string station2, double *flow, double *cost);

    private:
        Graph graph;
        unordered_map<string, Station*> nameToStation;
        map<string, set<string>> municipalityToStations;
        map<string, set<string>> districtToMunicipalities;

        void readStations(set<string> stations, set<string> lines);
        void readNetwork();
};


#endif //PROJECT_DA_DATABASE_H
