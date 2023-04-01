#ifndef PROJECT_DA_EDGE_H
#define PROJECT_DA_EDGE_H

#include "Node.h"

enum ServiceType {
    STANDARD,
    ALFA_PENDULAR
};

class Edge {
    public:
        Edge(Node* orig, Node* dest, int capacity, ServiceType service);

        Node* getDest() const;
        int getCapacity() const;
        ServiceType getService() const;
        Node* getOrig() const;
        Edge* getReverse() const;
        double getFlow() const;

        void setFlow(double flow);
        void setReverse(Edge* reverse);
        void setSelected(bool selected);

        bool operator<(const Edge& edge) const;

    private:
        Node* dest;
        int capacity;
        ServiceType service;

        bool selected = false;

        double flow;

        //Used for bidirectional edges
        Edge* reverse = nullptr;
        Node* orig;
};


#endif //PROJECT_DA_EDGE_H
