#ifndef PROJECT_DA_DATABASE_H
#define PROJECT_DA_DATABASE_H

#include "Graph.h"

class Database {
    public:
        Database();
    private:
        Graph graph;
        unordered_map<string, Station*> nameToStation;

        void readStations();
        void readNetwork();
};


#endif //PROJECT_DA_DATABASE_H
