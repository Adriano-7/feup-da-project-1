#include <map>
#include "NodeEdge.h"

/************************* Node  **************************/

/*
 * @brief Constructor of the Node class.
 * @details Time Complexity: O(1) Creates a node with a given station.
 * @param station - Station to be added to the node.
 * */
Node::Node(Station &station) : station(station) {}

/*
 * @brief Overloads the < operator.
 * @details Time Complexity: O(1) Compares the distance of two nodes.
 * @param node - Node to be compared.
 * */
bool Node::operator<(Node &node) {
    return this->distance < node.distance;
}

/*
 * @brief Returns the station of the node.
 * @details Time Complexity: O(1)
 * @return Reference to the station of the node.
 * */
Station& Node::getStation() {
    return station;
}

/*
 * @brief Returns the adjacency vector of the node.
 * @details Time Complexity: O(1)
 * @return Vector of edges that are adjacent to the node.
 * */
vector<Edge *> Node::getAdj() {
    return adj;
}

/*
 * @brief Returns the visited status of the node.
 * @details Time Complexity: O(1)
 * @return True if the node has been visited, false otherwise.
 * */
bool Node::isVisited() {
    return visited;
}

/*
 * @brief Returns the distance of the node.
 * @details Time Complexity: O(1)
 * @return Distance of the node.
 * */
double Node::getDistance() {
    return distance;
}

/*
 * @brief Returns the edge that leads to the parent of the node.
 * @details Time Complexity: O(1)
 * @return Edge that leads to the parent of the node.
 * */
Edge* Node::getPath() {
    return path;
}

/*
 * @brief Returns the incoming edges of the node.
 * @details Time Complexity: O(1)
 * @return Vector of pointers to the edges that lead to the node.
 * */
vector<Edge *> Node::getIncoming() {
    return incoming;
}

/*
 * @brief Returns the name of the station of the node.
 * @details Time Complexity: O(1)
 * @return Name of the station of the node.
*/
string Node::getStationName() {
    return station.getName();
}

/*
 * @brief Sets the visited status of the node.
 * @details Time Complexity: O(1)
 * @param visited - True to set the node as visited, false otherwise.
*/
void Node::setVisited(bool visited) {
    this->visited = visited;
}

/*
 * @brief Sets the distance of the node.
 * @details Time Complexity: O(1)
 * @param distance - Distance to be set.
 */
void Node::setDistance(double distance) {
    this->distance = distance;
}

/*
 * @brief Sets the edge that leads to the parent of the node.
 * @details Time Complexity: O(1)
 * @param path - Edge that leads to the parent of the node.
*/
void Node::setPath(Edge *path) {
    this->path = path;
}

/**
 * @brief Creates a edge from the current node to a destination node with a given capacity and service type.
 * @details Time Complexity: O()
 * @param dest - Destination node.
 * @param capacity - Capacity of the edge.
 * @param service - Service type of the edge.
 * @return Pointer to the edge created.
 */
Edge* Node::addEdge(Node *dest, int capacity, ServiceType service) {
    Edge *edge = new Edge(this, dest, capacity, service);
    adj.push_back(edge);
    dest->incoming.push_back(edge);
    return edge;
}
/**
 * @brief Removes an edge from the adjacency vector of the current node.
 * @details Time Complexity: O()
 * @param dest
 * @return
 */
bool Node::removeEdgeTo(Node *dest) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()){
        Edge *edge = *it;
        if (edge->getDest()->getStationName() == dest->getStationName()){
            it = adj.erase(it);
            removedEdge = true;
        }
        else
            it++;
    }
    return removedEdge;
}

/********************** Edge  ****************************/

/*
 * @brief Constructor of the Edge class.
 * @details Time Complexity: O(1) Creates an edge with a given origin, destination, capacity and service type.
 * */
Edge::Edge(Node *orig, Node *dest, int capacity, ServiceType service): orig(orig), dest(dest), capacity(capacity), service(service) {}

/*
 * @brief Returns a pointer to the destination node of the edge.
 * @details Time Complexity: O(1)
 * */
Node *Edge::getDest(){return this->dest;}

/*
 * @brief Returns the capacity of the edge.
 * @details Time Complexity: O(1)
*/
int Edge::getCapacity() {
    return this->capacity;
}

/*
 * @brief Returns the service type of the edge.
 * @details Time Complexity: O(1)
*/
ServiceType Edge::getService() {
    return this->service;
}

/*
 * @brief Returns the origin node of the edge.
 * @details Time Complexity: O(1)
*/
Node* Edge::getOrig() {
    return this->orig;
}

/*
 * @brief Returns the reverse edge of the current edge.
 * @details Time Complexity: O(1)
*/
Edge* Edge::getReverse() {
    return this->reverse;
}

/*
 * @brief Returns the flow of the current edge.
 * @details Time Complexity: O(1)
*/
double Edge::getFlow() {
    return this->flow;
}

/*
 * @brief Returns the cost of the current edge (2 for standard, 4 for alfa-pendular)
 * @details Time Complexity: O(1)
 */
double Edge::getCostService(){
    map<ServiceType, double> serviceCosts = {{ServiceType::STANDARD,2}, {ServiceType::ALFA_PENDULAR, 4}};
    return serviceCosts[this->getService()];
}

/*
 * @brief Sets the flow of the current edge.
 * @details Time Complexity: O(1)
 * @param flow - Flow to be set.
*/
void Edge::setFlow(double flow) {
    this->flow = flow;
}

/*
 * @brief Sets the reverse edge of the current edge.
 * @details Time Complexity: O(1)
 * @param reverse - Reverse edge to be set.
*/
void Edge::setReverse(Edge* reverse) {
    this->reverse = reverse;
}

/*
 * @brief Overload of the < operator (alphabetical order)
*/
bool Edge::operator<(Edge& edge) {
    return this->orig->getStation().getName() < edge.orig->getStation().getName();
}

/*
 * @brief Sets the capacity of the current edge.
 * @details Time Complexity: O(1)
 * @param capacity - Capacity to be set.
*/
void Edge::setCapacity(int capacity) {
    this->capacity = capacity;
}