#ifndef PROJECT_DA_NODE_H
#define PROJECT_DA_NODE_H

#include "Station.h"
#include "Edge.h"
#include <vector>

class Node {
    public:
        Node(Station& station);
        bool operator<(const Node& node) const;

        Station& getStation() const;
        vector<Edge *> getAdj() const;
        bool isVisited() const;
        bool isProcessing() const;
        unsigned int getIndegree() const;
        double getDistance() const;
        Edge *getPath() const;
        vector<Edge *> getIncoming() const;

        void setVisited(bool visited);
        void setProcessing(bool processing);
        void setIndegree(unsigned int indegree);
        void setDistance(double distance);
        void setPath(Edge *path);


    private:
        Station& station;
        vector<Edge *> adj;

        bool visited = false;
        bool processing = false;
        unsigned int indegree;
        double distance = 0;

        Edge *path = nullptr;
};


#endif //PROJECT_DA_NODE_H
