#include "NodeEdge.h"

/************************* Node  **************************/

Node::Node(Station &station) : station(station) {}

bool Node::operator<(Node &node) {
    return this->distance < node.distance;
}

Station &Node::getStation() {
    return station;
}

vector<Edge *> Node::getAdj() {
    return adj;
}

bool Node::isVisited() {
    return visited;
}

bool Node::isProcessing() {
    return processing;
}

unsigned int Node::getIndegree() {
    return indegree;
}

double Node::getDistance() {
    return distance;
}

Edge *Node::getPath() {
    return path;
}

vector<Edge *> Node::getIncoming() {
    return incoming;
}

string Node::getStationName() {
    return station.getName();
}

void Node::setVisited(bool visited) {
    this->visited = visited;
}

void Node::setProcessing(bool processing) {
    this->processing = processing;
}

void Node::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

void Node::setDistance(double distance) {
    this->distance = distance;
}

void Node::setPath(Edge *path) {
    this->path = path;
}

Edge * Node::addEdge(Node *dest, int capacity, ServiceType service) {
    Edge *edge = new Edge(this, dest, capacity, service);
    adj.push_back(edge);
    dest->incoming.push_back(edge);
    return edge;
}

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
}

/********************** Edge  ****************************/

Edge::Edge(Node *orig, Node *dest, int capacity, ServiceType service): orig(orig), dest(dest), capacity(capacity), service(service) {}

Node *Edge::getDest()
{
    return this->dest;
}

int Edge::getCapacity() {
    return this->capacity;
}

bool Edge::isSelected() {
    return this->selected;
}

ServiceType Edge::getService() {
    return this->service;
}

Node* Edge::getOrig() {
    return this->orig;
}

Edge* Edge::getReverse() {
    return this->reverse;
}

double Edge::getFlow() {
    return this->flow;
}

void Edge::setFlow(double flow) {
    this->flow = flow;
}

void Edge::setReverse(Edge* reverse) {
    this->reverse = reverse;
}

void Edge::setSelected(bool selected) {
    this->selected = selected;
}

bool Edge::operator<(Edge& edge) {
    return this->orig->getStation().getName() < edge.orig->getStation().getName();
}

