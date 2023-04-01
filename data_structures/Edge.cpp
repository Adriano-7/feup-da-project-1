#include "Edge.h"

Edge::Edge(Node* orig, Node* dest, int capacity, ServiceType service): orig(orig), dest(dest), capacity(capacity), service(service) {}

Node* Edge::getDest() const {return this->dest;}
int Edge::getCapacity() const {return this->capacity;}
ServiceType Edge::getService() const {return this->service;}
Node* Edge::getOrig() const {return this->orig;}
Edge* Edge::getReverse() const {return this->reverse;}
double Edge::getFlow() const {return this->flow;}

void Edge::setFlow(double flow) {this->flow = flow;}
void Edge::setReverse(Edge* reverse) {this->reverse = reverse;}
void Edge::setSelected(bool selected) {this->selected = selected;}

bool Edge::operator<(const Edge& edge) const {
    return this->orig->getStation().getName() < edge.orig->getStation().getName();
}

