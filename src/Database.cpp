#include "headers/Database.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

void Database::loadWithoutFilters() {
    readStations({}, {});
    readNetwork();
}

void Database::loadWithFilters(set<string> stations, set<string> lines) {
    readStations(stations, lines);
    readNetwork();
}

Station& Database::getStation(int id) {
    if (id < 0 || id >= graph.getNumNodes()) {
        cout << "Invalid station id" << endl;
        exit(1);
    }
    return graph.getNode(id)->getStation();
}

vector<pair<Node *, Node *>> Database::maxFlowAllPairs(int *maxFlow){
    return graph.maxFlowAllPairs(maxFlow);
}

int Database::getMaxFlowBetweenStations(int station1, int station2) {
    return graph.EdmondsKarp(graph.getNode(station1), graph.getNode(station2));
}

void Database::readStations(set<string> stations, set<string> lines) {
    ifstream file("../data/stations.csv");
    file.ignore(1000, '\n'); // ignore first line

    if (!file.is_open()) {
        cout << "Error opening stations.csv" << endl;
        return;
    }
    int id = -1;
    string line;
    while (getline(file, line)) {
        vector<string> fields;
        istringstream stream(line);
        string field;
        while (getline(stream, field, ';')) {
            fields.push_back(field);
        }
        if (fields.size() != 5) {
            cout << "The following line is invalid: " << line << endl;
            return;
        }


        id++;
        string name = fields[0];
        string district = fields[1];
        string municipality = fields[2];
        string township = fields[3];
        string line = fields[4];

        if (!stations.empty() && stations.find(name) == stations.end()) {
            continue;
        }

        if (!lines.empty() && lines.find(line) == lines.end()) {
            continue;
        }

        Station *station = new Station(id,name, district, municipality, township, line);
        graph.addNode(*station);
        nameToStation[name] = station; //TO REMOVE

        if(district.empty() || municipality.empty()) continue;
        if(municipalitiesByDistrict.find(district) == municipalitiesByDistrict.end()){
            vector<string> municipalities;
            municipalities.push_back(municipality);
            municipalitiesByDistrict[district] = municipalities;
            districts.push_back(district);
        }
        else{
            if(find(municipalitiesByDistrict[district].begin(), municipalitiesByDistrict[district].end(), municipality) == municipalitiesByDistrict[district].end())
                municipalitiesByDistrict[district].push_back(municipality);
        }

        if(stationsByMunicipality.find(municipality) == stationsByMunicipality.end()){
            vector<int> ids;
            ids.push_back(station->getId());
            stationsByMunicipality[municipality] = ids;
        }
        else{
            if(find(stationsByMunicipality[municipality].begin(), stationsByMunicipality[municipality].end(), id) == stationsByMunicipality[municipality].end())
                stationsByMunicipality[municipality].push_back(id);
        }

    }
    file.close();
    return;
}

void Database::readNetwork() {
    ifstream file("../data/network.csv");
    file.ignore(1000, '\n'); // ignore first line

    if (!file.is_open()) {
        cout << "Error opening network.csv" << endl;
        return;
    }

    string line;
    int lineCount = 0;
    while (getline(file, line)) {
        lineCount++;
        vector<string> fields;
        istringstream stream(line);
        string field;
        while (getline(stream, field, ';')) {
            fields.push_back(field);
        }
        if (fields.size() != 4) {
            cout << "Line" << lineCount << " is invalid: " << line << endl;
        }

        string orig = fields[0];
        string dest = fields[1];
        int capacity = stoi(fields[2]);
        string serviceType = fields[3];
        ServiceType service;
            if(serviceType == "STANDARD")
                service = STANDARD;
            else if(serviceType == "ALFA PENDULAR")
                service = ALFA_PENDULAR;
            else{
                cout << "Line " << lineCount << " service type is invalid: " << line << endl;
            }

        Station *origStation = nameToStation[orig];
        Station *destStation = nameToStation[dest];

        if (origStation == nullptr || destStation== nullptr)
            continue;
        //I want the nodes that have this station
        Node* origNode = graph.getNode(origStation->getId());
        Node* destNode = graph.getNode(destStation->getId());

        graph.addBidirectionalEdge(origNode, destNode, capacity, service);
    }
    file.close();

    return;
}

void Database::printNodes() {
    vector<Node *> nodes = graph.getNodeVector();
    for (auto node: nodes) {
        cout << node->getStation().getName() << endl;
    }
}

void Database::printEdges(){
    int count = 0;
     for(auto node: graph.getNodeVector()){
         for (auto edge: node->getAdj()) {
             cout << node->getStation().getName() << " " << edge->getDest()->getStation().getName() << " " << edge->getCapacity() << " " << edge->getService() << endl;
             count++;}
     }
     cout << count << endl;
}

int Database::getMaxTrainsStation(int id) {
    return graph.maxTrains(graph.getNode(id));
}

vector<string> Database::getDistricts() {
    return districts;
}

vector<string> Database::getMunicipalities(string district) {
    return municipalitiesByDistrict[district];
}

vector<int> Database::getStations(string municipality) {
    return stationsByMunicipality[municipality];

}
