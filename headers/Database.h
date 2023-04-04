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

        int getMaxFlowBetweenStations(string station1, string station2);
        int getMaxTrainsStation(string station);


private:
        Graph graph;
        unordered_map<string, Station*> nameToStation;

        void readStations(set<string> stations, set<string> lines);
        void readNetwork();

};


#endif //PROJECT_DA_DATABASE_H
