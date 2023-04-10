#include "Graph.h"
#include "Station.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <limits>

using namespace std;

/**
 * @brief Adds a node to the graph.
 * @details Time Complexity: O(log n), where n is the number of nodes in the graph.
 * @param station - The station of the node to be added.
 * @return True if the node was added successfully, false otherwise.
 */
bool Graph::addNode(Station& station) {
    if (nodes.find(station.getName()) != nodes.end()){
        cout << "Station " << station.getName() << " already exists." << endl;
        return false;
    }

    nodes[station.getName()] = new Node(station);
    return true;
}

/**
 * @brief Adds a unidirectional edge to the graph.
 * @details Time Complexity: O(1)
 * @param sourceNode - The source node.
 * @param destNode - The destination node.
 * @param capacity - Number of trains that can pass through the edge.r
 * @param service - Which can be either Standard or AlfaPendular
 * @return True if the edge was added successfully, false otherwise.
 */
bool Graph::addEdge(Node* sourceNode, Node* destNode, int capacity, ServiceType service) {
    if (sourceNode== nullptr){
        cout << "Station " << sourceNode->getStation().getName() << " does not exist." << endl;
        return false;
    }

    if (destNode== nullptr){
        cout << "Station " << destNode->getStation().getName() << " does not exist." << endl;
        return false;
    }

    sourceNode->addEdge(destNode, capacity, service);
    return true;
}

/**
 * @brief Adds a Bidirectional Edge to the graph.
 * @details Time Complexity: O(1)
 * @param sourceNode - The source node.
 * @param destNode - The destination node.
 * @param capacity - Number of trains that can pass through the edge.r
 * @param service - Which can be either Standard or AlfaPendular
 * @return
 */
bool Graph::addBidirectionalEdge(Node* sourceNode, Node* destNode, int capacity, ServiceType service) {
    if (sourceNode== nullptr){
        cout << "Station " << sourceNode->getStation().getName() << " does not exist." << endl;
        return false;
    }

    if (destNode== nullptr){
        cout << "Station " << destNode->getStation().getName() << " does not exist." << endl;
        return false;
    }

    Edge* e1 = sourceNode->addEdge(destNode, capacity, service);
    Edge* e2 = destNode->addEdge(sourceNode, capacity, service);

    e1->setReverse(e2);
    e2->setReverse(e1);

    return true;
}

/**
 * @brief Removes a node from the graph.
 * @details Time Complexity: O(E+I), where E is the number of outgoing edges and I is the number of incoming edges.
 * @param node - The node to be removed.
 * @return True if the node was removed successfully, false otherwise.
 */
bool Graph::eraseNode(Node *node) {
    for (Edge *e: node->getAdj()) {
        Node *w = e->getDest();
        if (!node->removeEdgeTo(w)) {
            return false;
        }
    }
    nodes.erase(node->getStation().getName());
    return true;
}

/**
 * @brief Given the name of a station, returns the node associated with it.
 * @details Time Complexity: O(log n), where n is the number of nodes in the graph.
 * @param stationName - The name of the station.
 * @return A pointer to the node associated with the station, nullptr if the station does not exist.
 */
Node* Graph::getNode(string stationName) {
    if (nodes.find(stationName) != nodes.end())
        return nodes[stationName];
    else
        return nullptr;
}

/**
 * @brief Given a station, returns the node associated with it.
 * @details Time Complexity: O(log n), where n is the number of nodes in the graph.
 * @param station - A pointer to the station.
 * @return A pointer to the node associated with the station, nullptr if the station does not exist.
 */
Node* Graph::getNode(Station* station) {
    return getNode(station->getName());
}

/**
 * @brief Finds paths between the source and the destination.
 * @details Time Complexity: O(E+V), where E is the number of edges and V is the number of nodes.
 * @param source - The source node.
 * @param dest - The destination node.
 * @return True if there is a path between the source and the destination, false otherwise.
 */
bool Graph::bfs(Node* source, Node* dest){
    for(pair<string, Node*> nodePair : nodes) {
        Node* node = nodePair.second;
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

/**
 * @brief Finds the shortest path between the source and the destination using the cost of the service (and the capacity).
 * @details Time Complexity: O((V+E log V), where E is the number of edges and V is the number of nodes.
 * Taking into account the cost of Service and the number of trains that can pass through the edge finds the shortest path between the source and the destination.
 * @param source - The source node.
 * @param dest - The destination node.
 * @return The cost of the path.
 */
double Graph::dijkstra(Node* source, Node* dest){
    queue<Node*> q;

    for(pair<string, Node*> nodePair : nodes) {
        Node* node = nodePair.second;
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

        if(v == dest){ break;}
        for(Edge* e: v->getAdj()){
            Node* w = e->getDest();
            if(!w->isVisited()){
                double distance = v->getDistance() + e->getCostService()*e->getCapacity();
                if(distance < w->getDistance()){
                    w->setDistance(distance);
                    w->setPath(e);
                    q.push(w);
                }
            }
        }
    }

    return dest->getPath() == nullptr ? -1 : dest->getDistance();
}

/**
 * @brief Edmonds Karp - Finds the maximum flow in the network flow graph
 * @details Time Complexity: O(VE^2), where E is the number of edges and V is the number of nodes.
 * Using BFS finds the maximum flow in the network flow graph between the source and the destination.
 * @param source - Source node
 * @param dest - Destination node
 * @return The maximum flow.
 */
int Graph::EdmondsKarp(Node* source, Node* dest){
    if(source == nullptr || dest == nullptr || source == dest){
        cout << "Invalid source or destination" << endl;
        return -1;
    }

    for(pair<string, Node*> nodePair : nodes) {
        Node* node = nodePair.second;
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

/**
 * @brief Uses the Dijkstra algorithm to find the shortest path (cost of service) and then calculates the bottleneck flow.
 * @details Time Complexity: O((V+E log V), where E is the number of edges and V is the number of nodes.
 * @param source - Source node
 * @param dest - Destination node
 * @param flow - Pointer to that will be filled with the bottleneck flow
 * @param costService - Pointer to that will be filled with the cost of the service
 * @return Stack with the edges of the path.
 */
stack<Edge*> Graph::BottleneckDijkstra(Node* source, Node* dest, double* flow, double* costService){
    stack<Edge*> path;
    if(source == nullptr || dest == nullptr || source == dest){
        cout << "Invalid source or destination" << endl;
        *flow = -1;
        return path;
    }

    *costService = dijkstra(source, dest);
    if(*costService==-1){
        *flow = -1;
        *costService = -1;
        return path;
    }

    double pathFlow = numeric_limits<double>::infinity();
    for(Node* v = dest; v != source; ) {
        Edge* edge = v->getPath();

        if(edge== nullptr){
            *flow = -1;
            return path;
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

    *costService = 0;
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
        path.push(e);
        *costService += e->getCostService();
    }

    *flow = pathFlow;
    return path;
}

/**
 * @brief Iterates through all pairs of nodes in the network and uses the Edmonds Karp algorithm to find the maximum flow between them.
 * @details Time Complexity: O(V^3E^2), where E is the number of edges and V is the number of nodes.
 * @param maxFlow - Pointer to the variable that will store the maximum flow.
 * @return A vector with the pairs of nodes that have the maximum flow.
 */
vector<pair<Node *, Node *>> Graph::maxFlowAllPairs(int *maxFlow) {
    *maxFlow = INT_MIN;
    vector<pair<Node *, Node *>> result;

    for(auto it1 = nodes.begin(); it1 != nodes.end(); it1++){
        for(auto it2 = it1; it2 != nodes.end(); it2++){
            if(it1==it2) { continue;}
            else{
                int curFlow = EdmondsKarp(it1->second, it2->second);
                if(curFlow > *maxFlow){
                    result.erase(result.begin(), result.end());
                    *maxFlow = curFlow;
                    result.push_back(make_pair(it1->second, it2->second));
                }
                else if(curFlow == *maxFlow){
                    result.push_back(make_pair(it1->second, it2->second));
                }
            }
        }
    }

    if(*maxFlow == INT_MIN) { *maxFlow = 0;}
    return result;
}

/**
 * @brief Calculates the sum of the maximum flows between all pairs of nodes in the input set.
 * @details Time Complexity: O(V E^2 * P^2), where E is the number of edges, V is the number of nodes in the graph and P is the number of nodes in the input set.
 * @param nodes - Set of nodes to calculate the sum of the maximum flows between them.
 * @param sumFlow - Pointer to the variable that will store the sum of the maximum flows.
 */
void Graph::sumSomePairsFlow(set<Node*> nodes, int* max_flow) {
    int sum = 0;
    for(auto it1 = nodes.begin(); it1 != nodes.end(); it1++){
        for(auto it2 = it1; it2 != nodes.end(); it2++){
            if(it1==it2) { continue;}
            int curFlow = EdmondsKarp(*it1, *it2);
            sum += curFlow;
        }
    }
    *max_flow = sum;
}

/**
 * @brief Calculates the maximum incoming flow to the input node.
 * @details Time Complexity: O(V E^2), where E is the number of edges and V is the number of nodes.
 * Creates a super source node and connects it to all nodes that have only one outgoing edge. Then uses the Edmonds Karp algorithm to find the maximum flow between the super source and the input node.
 * @param node - Node to calculate the maximum incoming flow.
 * @return The maximum incoming flow to the input node.
 */
int Graph::maxIncomingFlow(Node* node){
    Station* superSourceStation = new Station("SuperSource", "", "", "", "");
    Node* superSource = new Node(*superSourceStation);

    for(pair<string, Node*> nodePair : nodes) {
        Node* nodeIt = nodePair.second;
        if(nodeIt != node && nodeIt->getAdj().size() == 1) {
            this->addEdge(superSource, nodeIt, INT_MAX, ServiceType::NONE);
        }
    }

    int max =  EdmondsKarp(superSource, node);
    if(!eraseNode(superSource)){ cout << "Error erasing super source" << endl;}
    return max;
}

/**
 * @brief Check if two nodes are adjacent.
 * @details Time Complexity: O(E), where E is the number of adjacent edges.
 * Checks if two nodes are adjacent, given the source and destination nodes of the edge. And updates the var edgeCapacity with the capacity of the edge.
 * @param source - Source node of the edge.
 * @param dest - Destination node of the edge.
 * @param edgeCapacity - Pointer to the variable that will store the capacity of the edge.
 * @return True if the nodes are adjacent, false otherwise.
 */
bool Graph::checkConnection(Node* source, Node* dest, int& curCapacity) {
    for (auto edge : source->getAdj()) {
        if (edge->getDest() == dest) {
            curCapacity = edge->getCapacity();
            return true;
        }
    }
    return false;
}

/**
 * @brief Changes the capacity of an edge.
 * @details Time Complexity: O(E), where E is the number of adjacent edges.
 * Changes the capacity of the edges between two nodes with the new capacity.
 * @param source - Source node of the edge.
 * @param dest - Destination node of the edge.
 * @param newCapacity - New capacity of the edge.
 */
void Graph::changeCapacity(Node* source, Node* dest, int newCapacity) {
    for(Edge* edge: source->getAdj()){
        if(edge->getDest() == dest){
            edge->setCapacity(newCapacity);
            edge->getReverse()->setCapacity(newCapacity);
        }
    }
}