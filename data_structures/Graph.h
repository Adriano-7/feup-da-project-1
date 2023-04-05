#ifndef PROJECT_DA_GRAPH_H
#define PROJECT_DA_GRAPH_H

#include <string>
#include <set>
#include <map>
#include "Station.h"
#include "NodeEdge.h"
#include "ServiceType.h"
#include <stack>
using namespace std;
/**
 * @brief Class that processes everything that uses graphs.
 */
class Graph {
    public:
        bool addNode(Station& station);
        bool addEdge(Node* source, Node* dest, int capacity, ServiceType service);
        bool addBidirectionalEdge(Node* sourceNode, Node* destNode, int capacity, ServiceType service);
        bool eraseNode(Node* node);

        Node* getNode(string stationName);
        Node* getNode(Station* station);

        bool bfs(Node* source, Node* dest);
        double dijkstra(Node* source, Node* dest);
        int EdmondsKarp(Node* source, Node* dest);
        stack<Edge*> FordFulkersonDijkstra(Node* source, Node* dest, double *flow, double *cost);
        vector<pair<Node*, Node*>> maxFlowAllPairs(int *max_flow);
        void sumSomePairsFlow(set<Node*> nodes, int *max_flow);
        int maxIncomingFlow(Node* node);

        bool checkConnection(Node* source, Node* dest, int& curCapacity);
        void changeCapacity(Node* source, Node* dest, int newCapacity);

private:
       map<string, Node*> nodes;
};

#endif //PROJECT_DA_GRAPH_H
