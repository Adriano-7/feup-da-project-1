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
        bool addNode(Station& station);
        bool addEdge(Node* source, Node* dest, int capacity, ServiceType service);
        bool eraseNode(Node* node);

        Node* getNode(string stationName);
        Node* getNode(Station* station);
        map<string, Node*> & getNodeMap();
        vector<pair<Node*, Node*>> maxFlowAllPairs(int *max_flow);

        int EdmondsKarp(Node* source, Node* dest);
        bool bfs(Node* source, Node* dest);
        vector<Node*> FordFulkersonDijkstra(Node* source, Node* dest, double *flow, double *cost);
        double dijkstra(Node* source, Node* dest);
        void sumSomePairsFlow(set<Node*> nodes, int *max_flow);
        int maxIncomingFlow(Node* node);


    private:
        map<string, Node*> nodes;
};

#endif //PROJECT_DA_GRAPH_H
