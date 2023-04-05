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
        bool addBidirectionalEdge(Node* sourceNode, Node* destNode, int capacity, ServiceType service);
        Node* getNode(int id);
        int getNumNodes();
        vector<Node*> & getNodeVector();
        vector<pair<Node*, Node*>> maxFlowAllPairs(int *max_flow);

        int EdmondsKarp(Node* source, Node* dest);
        bool bfs(Node* source, Node* dest);
        int maxTrains (Node* station);


    void maxSomePairs(vector<int> vector1, int *pInt);

private:
       vector<Node*> nodes;

};

#endif //PROJECT_DA_GRAPH_H
