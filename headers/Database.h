#ifndef PROJECT_DA_DATABASE_H
#define PROJECT_DA_DATABASE_H

#include "Graph.h"
#include <unordered_map>

class Database {
    public:
        Database() = default;
        void printNodes();
        void loadWithoutFilters();
        void loadWithFilters(set<string> stations, set<string> lines, ServiceType service);

    private:
        Graph graph;
        unordered_map<string, Station*> nameToStation;

        void readStations();
        void readStations(set<string> stations, set<string> lines);
        void readNetwork();
        void readNetwork(ServiceType services);
};


#endif //PROJECT_DA_DATABASE_H
