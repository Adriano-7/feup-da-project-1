#include "Graph.h"
#include "Station.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <limits>

using namespace std;

bool Graph::addNode(Station& station) {
    for(auto & node : nodes){
        if(node->getStation().getName() == station.getName()){
            return false;
        }
    }
    Node* node = new Node(station);
    nodes.push_back(node);

    return true;
}

bool Graph::addEdge(Node* sourceNode, Node* destNode, int capacity, ServiceType service) {
    if(sourceNode== nullptr){
        cout << "Station" << sourceNode->getStationName() << "does not exist." << endl;
        return false;
    }
    if(destNode== nullptr){
        cout << "Station" << destNode->getStationName() << "does not exist." << endl;
        return false;
    }
    sourceNode->addEdge(destNode, capacity, service);
    return true;
}

bool Graph::addBidirectionalEdge(Node* sourceNode, Node* destNode, int capacity, ServiceType service) {
    if(sourceNode== nullptr){
        cout << "Station" << sourceNode->getStationName() << "does not exist." << endl;
        return false;
    }

    if(destNode== nullptr){
        cout << "Station"<< destNode->getStationName() << " does not exist." << endl;
        return false;
    }

    Edge* e1 = sourceNode->addEdge(destNode, capacity, service);
    Edge* e2 = destNode->addEdge(sourceNode, capacity, service);

    e1->setReverse(e2);
    e2->setReverse(e1);

    return true;
}

Node* Graph::getNode(int id) {
    if (nodes[id] != nullptr)
        return nodes[id];
    else
        return nullptr;
}

int Graph::getNumNodes() {
    return nodes.size();
}

vector<Node*> & Graph::getNodeVector() {
    return nodes;
}

int Graph::EdmondsKarp(Node* source, Node* dest){
    if(source == nullptr || dest == nullptr || source == dest){
        cout << "Invalid source or destination" << endl;
        return -1;
    }

    for(Node*  node :  nodes) {
        for(Edge* e: node->getAdj()) {
            e->setFlow(0);
        }
    }

    int maxFlow = 0;
    while(bfs(source, dest)) {
        double pathFlow = numeric_limits<double>::infinity();
        for(Node* v = dest; v != source; ) {
            Edge* edge = v->getPath();
            if (edge->getDest() == v) {
                double residualCapacity = edge->getCapacity() - edge->getFlow();
                pathFlow = std::min(pathFlow, residualCapacity);
                v = edge->getOrig();
            }
            else{
                pathFlow = std::min(pathFlow, edge->getFlow());
                v = edge->getDest();
            }
        }
        for(Node* v = dest; v != source; ) {
            Edge* e = v->getPath();
            if (e->getDest() == v) {
                e->setFlow(e->getFlow() + pathFlow);
                v = e->getOrig();
            }
            else {
                e->setFlow(e->getFlow() - pathFlow);
                v = e->getDest();
            }
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

bool Graph::bfs(Node* source, Node* dest){
    for(Node* node : nodes) {
        node->setVisited(false);
        node->setPath(nullptr);
    }

    queue<Node *> q;
    source->setVisited(true);
    q.push(source);

    while (!q.empty() && !dest->isVisited()) {
        Node* v = q.front();
        q.pop();
        for(Edge* e: v->getAdj()) {
            Node* w = e->getDest();
            double residualCapacity = e->getCapacity() - e->getFlow();
            if (!w->isVisited() && residualCapacity > 0) {
                w->setVisited(true);
                w->setPath(e);
                q.push(w);
            }
        }
        for(Edge* e: v->getIncoming()) {
            Node* w = e->getOrig();
            double residualCapacity = e->getFlow();
            if (!w->isVisited() && residualCapacity > 0) {
                w->setVisited(true);
                w->setPath(e);
                q.push(w);
            }
        }
    }
    return dest->isVisited();
}

vector<pair<Node *, Node *>> Graph::maxFlowAllPairs(int *maxFlow) {
    *maxFlow = INT_MIN;
    vector<pair<Node *, Node *>> result;

     for(auto it1 = nodes.begin(); it1 != nodes.end(); it1++){
         for(auto it2 = it1; it2 != nodes.end(); it2++){
             if(it1==it2) { continue;}
             else{
                 int curFlow = EdmondsKarp(*it1, *it2);
                 //cout<< "Max flow from " << it1->second->getStation().getName() << " to " << it2->second->getStation().getName() << " is " << curFlow << endl;
                 if(curFlow > *maxFlow){
                     result.erase(result.begin(), result.end());
                     *maxFlow = curFlow;
                     result.push_back(make_pair(*it1, *it2));
                 }
                 else if(curFlow == *maxFlow){
                     result.push_back(make_pair(*it1, *it2));
                 }
             }
         }
     }

    return result;
}

int Graph::maxTrains(Node *station) {
    Station *superSourceStation = new Station(999, "SuperSource", "", "", "", "");
    Node* superSource = new Node(*superSourceStation);
    for(Node* node : nodes) {
            if (node != station && node->getAdj().size() == 1) {
                this->addEdge(superSource, node, INT_MAX, ServiceType::NONE);
            }
        }

    int max =  EdmondsKarp(superSource, station);
    delete superSourceStation;
    delete superSource;
    return max;
    }

void Graph::maxSomePairs(vector<int> stations, int *pInt) {
    int max = 0;
    for(int i = 0; i < stations.size(); i++){
        for(int j = i+1; j < stations.size(); j++){
            int curFlow = EdmondsKarp(nodes[stations[i]], nodes[stations[j]]);
            max += curFlow;
            }
        }

    *pInt = max;
}

vector<Node*> Graph::FordFulkersonDijkstra(Node* source, Node* dest, double* flow, double* cost){
    if(source == nullptr || dest == nullptr || source == dest){
        cout << "Invalid source or destination" << endl;
        flow = nullptr;
        return vector<Node*>{};
    }

    *cost = dijkstra(source, dest);
    if(*cost==-1){
        cout << "No path found" << "for source " << source->getStation().getName() << " and destination " << dest->getStation().getName() << endl;
        *flow = 0;
        *cost = -1;
        return vector<Node*>{};
    }

    double pathFlow = numeric_limits<double>::infinity();
    for(Node* v = dest; v != source; ) {
        Edge* edge = v->getPath();

        if(edge== nullptr){
            cout << "No path found" << "for source " << source->getStation().getName() << " and destination " << dest->getStation().getName() << endl;
            flow = nullptr;
            return vector<Node*>{};
        }

        if (edge->getDest() == v) {
            double residualCapacity = edge->getCapacity() - edge->getFlow();
            pathFlow = std::min(pathFlow, residualCapacity);
            v = edge->getOrig();
        }
        else{
            pathFlow = std::min(pathFlow, edge->getFlow());
            v = edge->getDest();
        }
    }

    vector<Node*> path;
    for(Node* v = dest; v != source;) {
        Edge* e = v->getPath();
        if (e->getDest() == v) {
            e->setFlow(e->getFlow() + pathFlow);
            v = e->getOrig();
        }
        else {
            e->setFlow(e->getFlow() - pathFlow);
            v = e->getDest();
        }
        path.push_back(v);
    }

    *flow = pathFlow;
    return path;
}

double Graph::dijkstra(Node* source, Node* dest){
    queue<Node*> q;
    map<ServiceType, double> serviceCosts = {{ServiceType::STANDARD,2}, {ServiceType::ALFA_PENDULAR, 4}};

    for(Node* node: nodes){
        node->setVisited(false);
        node->setPath(nullptr);
        node->setDistance(numeric_limits<double>::infinity());

        for(Edge* e: node->getAdj()) {
            e->setFlow(0);
        }
    }

    source->setDistance(0);
    q.push(source);
    vector<Node*> path;

    while(!q.empty()){
        Node* v = q.front();
        q.pop();
        v->setVisited(true);

        if(v == dest){
            break;
        }

        for(Edge* e: v->getAdj()){
            Node* w = e->getDest();
            if(!w->isVisited()){
                double distance = v->getDistance() + serviceCosts[e->getService()];
                if(distance < w->getDistance()){
                    w->setDistance(distance);
                    w->setPath(e);
                    q.push(w);
                }
            }
        }
    }

    if(dest->getPath() == nullptr){
        cout << "No path found" << "for source " << source->getStation().getName() << " and destination " << dest->getStation().getName() << endl;
        return -1;
    }
    return dest->getDistance();
}

bool Graph::checkConnection(int station1, int station2, int& curCapacity) {
    Node* source = nodes[station1];
    Node* dest = nodes[station2];
    for (auto edge : source->getAdj()) {
        if (edge->getDest() == dest) {
            curCapacity = edge->getCapacity();
            return true;
        }
    }
    return false;
}

void Graph::changeCapacity(int station1, int station2, int newCapacity) {
    Node* source = nodes[station1];
    Node* dest = nodes[station2];
    for (auto edge : source->getAdj()) {
        if (edge->getDest() == dest) {
            edge->setCapacity(newCapacity);
        }
    }
}
