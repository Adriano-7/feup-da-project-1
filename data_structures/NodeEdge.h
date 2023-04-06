#ifndef PROJECT_DA_NODEEDGE_H
#define PROJECT_DA_NODEEDGE_H

#include "Station.h"

#include <vector>
#include "ServiceType.h"

class Edge;
/**
 * @brief Represents a node in the graph. Contains a station and a vector of adjacent edges.
 */
class Node{
    public:
        Node(Station& station);
        bool operator<(Node& node);

        Station& getStation();
        vector<Edge *>& getAdj();
        bool isVisited();
        double getDistance();
        Edge *getPath();
        vector<Edge *>& getIncoming();
        string getStationName();

        void setVisited(bool visited);
        void setDistance(double distance);
        void setPath(Edge *path);
        Edge *addEdge(Node *dest, int capacity, ServiceType service);
        bool removeEdgeTo(Node *dest);


    private:
        Station& station;
        vector<Edge *> adj;
        vector<Edge *> incoming;

        bool visited = false;
        double distance = 0;
        Edge *path = nullptr;
};
/**
 * @brief Represents an edge in the graph. Contains a destination node, a capacity, a service type and a flow.
 */
class Edge {
public:
    Edge(Node *orig, Node *dest, int capacity, ServiceType service);

    Node* getDest();
    int getCapacity();
    ServiceType getService();
    Node* getOrig();
    Edge* getReverse();
    double getFlow();
    double getCostService();

    void setFlow(double flow);
    void setReverse(Edge* reverse);
    void setCapacity(int capacity);

    bool operator<(Edge& edge);

private:
    Node * dest;
    int capacity;
    ServiceType service;

    double flow;

    Edge *reverse = nullptr;
    Node *orig;
};



#endif //PROJECT_DA_NODEEDGE_H
