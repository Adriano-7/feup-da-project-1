#ifndef PROJECT_DA_GRAPH_H
#define PROJECT_DA_GRAPH_H

#include <string>
#include <set>
#include <unordered_map>
#include "Station.h"
using namespace std;

class Graph {
    private:
        set<Node *> nodeSet;

    public:
        Graph() = default;

        void addNode(Station& station);
        void addEdge(Station& source, Station& dest, int capacity, string service);
};

#endif //PROJECT_DA_GRAPH_H
