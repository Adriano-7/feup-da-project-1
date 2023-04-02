#include "Graph.h"
#include "Station.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>

using namespace std;

bool Graph::addNode(Station& station) {
    if (nodes.find(station.getName()) != nodes.end()){
        cout << "Station " << station.getName() << " already exists." << endl;
        return false;
    }

    nodes[station.getName()] = new Node(station);
    return true;
}

bool Graph::addEdge(Station& source, Station& dest, int capacity, ServiceType service) {
    Node* sourceNode = nodes[source.getName()];
    Node* destNode = nodes[dest.getName()];

    if (sourceNode== nullptr){
        cout << "Station " << source.getName() << " does not exist." << endl;
        return false;
    }

    if (destNode== nullptr){
        cout << "Station " << dest.getName() << " does not exist." << endl;
        return false;
    }


    Edge* e1 = sourceNode->addEdge(destNode, floor(capacity/2.0), service);
    Edge* e2 = destNode->addEdge(sourceNode, ceil(capacity/2.0), service);

    e1->setReverse(e2);
    e2->setReverse(e1);

    return true;
}

Node* Graph::getNode(string stationName) {
    Node* node = nodes["Afife"];
    if (node == nullptr)
        cout << "Station " << stationName << " does not exist." << endl;
        return nullptr;

    return node;
}

int Graph::getNumNodes() {
    return nodes.size();
}

map<string, Node*> & Graph::getNodeMap() {
    return nodes;
}

int Graph::maxFlow(Node* source, Node* dest){
    int maxFlow = 0;
    for(Edge* edge : source->getAdj())
        edge->setFlow(0);

    while (bfs(source, dest)){
        int pathFlow = INT_MAX;

        for (Node* node = dest; node != source; node = node->getPath()->getOrig()){
            int residualCapacity = node->getPath()->getCapacity() -node->getPath()->getFlow();
            pathFlow = min(pathFlow, residualCapacity);
        }

        for (Node* v = dest; v != source; v = v->getPath()->getOrig()){
            v->getPath()->addFlow(pathFlow);
            v->getPath()->getReverse()->removeFlow(pathFlow);
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

bool Graph::bfs(Node* source, Node* dest){
    queue<Node*> q;

    for (pair<string, Node*> nodePair : nodes){
        Node* node = nodePair.second;

        node->setVisited(false);
        node->setPath(nullptr);
    }

    q.push(source);
    source->setVisited(true);
    while (!q.empty()){
        Node* v = q.front();
        q.pop();

        for (Edge* edge : v->getAdj()){
            if (!edge->getDest()->isVisited() && edge->getCapacity() > edge->getFlow()){
                edge->getDest()->setVisited(true);
                edge->getDest()->setPath(edge);
                q.push(edge->getDest());
            }
        }
    }
    return dest->isVisited();
}

