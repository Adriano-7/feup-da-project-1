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

Station* Database::getStation(string stationName) {
    return nameToStation[stationName];
}

vector<pair<Node *, Node *>> Database::maxFlowAllPairs(int *maxFlow){
    return graph.maxFlowAllPairs(maxFlow);
}

map<string, set<string>> Database::getDistrictToMunicipalities(){
    return districtToMunicipalities;
}

set<string> Database::getStationsFromMunicipality(string municipality){
    return municipalityToStations[municipality];
}

int Database::getMaxFlowBetweenStations(Station* station1, Station* station2) {
    return graph.EdmondsKarp(graph.getNode(station1), graph.getNode(station2));
}

vector<Node*> Database::getMinCostFlow(Station* station1, Station* station2, double *flow, double *cost){
    return graph.FordFulkersonDijkstra(graph.getNode(station1), graph.getNode(station2), flow, cost);
}

int Database::getMaxTrainsStation(Station* station){
    return graph.maxIncomingFlow(graph.getNode(station));
}

vector<pair<string, int>> Database::getTopMunicipalities(int k){
    vector<pair<string, int>> res;
    for(pair<string, set<string>> municipality : municipalityToStations){
        int curFlow;
        set<Node*> nodes;
        for(string station : municipality.second){
            nodes.insert(graph.getNode(station));
        }
        graph.sumSomePairsFlow(nodes, &curFlow);
        res.emplace_back(municipality.first, curFlow);
    }
    sort(res.begin(), res.end(), [](pair<string, int> a, pair<string, int> b){
        return a.second > b.second;
    });

    vector<pair<string, int>> topMunicipalities;
    for(int i = 0; i < k; i++){
        topMunicipalities.push_back(res[i]);
    }

    return topMunicipalities;
}

vector<pair<string, int>> Database::getTopDistricts(int k){
    vector<pair<string, int>> res;
    for (pair<string, set<string>> district: districtToMunicipalities) {
        int curFlow;
        set<Node*> nodes;
        for(auto &municipality: district.second){
            for(auto &station: municipalityToStations[municipality]){
                nodes.insert(graph.getNode(station));
            }
        }
        graph.sumSomePairsFlow(nodes, &curFlow);
        res.emplace_back(district.first, curFlow);
    }
    sort(res.begin(), res.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
        return a.second > b.second;
    });
    vector<pair<string, int>> topDistricts;
    for (int i = 0; i < k; i++) {
        topDistricts.push_back(res[i]);
    }
    return topDistricts;
}

void Database::readStations(set<string> stations, set<string> lines) {
    ifstream file("../data/stations.csv");
    file.ignore(1000, '\n'); // ignore first line

    if (!file.is_open()) {
        cout << "Error opening stations.csv" << endl;
        return;
    }

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

        Station *station = new Station(name, district, municipality, township, line);
        graph.addNode(*station);
        nameToStation[name] = station;
        municipalityToStations[municipality].insert(name);
        districtToMunicipalities[district].insert(municipality);
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

        graph.addEdge(graph.getNode(orig), graph.getNode(dest), capacity, service);
    }
    file.close();
    return;
}