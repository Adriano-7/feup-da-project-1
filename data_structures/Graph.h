#ifndef PROJECT_DA_GRAPH_H
#define PROJECT_DA_GRAPH_H

#include <string>
#include <set>
#include <map>
#include "Station.h"
#include "NodeEdge.h"
#include "ServiceType.h"
using namespace std;

class Graph {
    public:
        Node* getNode(string stationName);
        bool addNode(Station& station);

        bool addEdge(Station& source, Station& dest, int capacity, ServiceType service);

        int getNumNodes();
    private:
        map<string, Node*> nodes;
};

#endif //PROJECT_DA_GRAPH_H
