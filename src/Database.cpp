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

vector<Node*> Database::getMinCostFlow(int station1, int station2, double *flow, double *cost){
    return graph.FordFulkersonDijkstra(graph.getNode(station1), graph.getNode(station2), flow, cost);
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

        Node* origNode = graph.getNode(origStation->getId());
        Node* destNode = graph.getNode(destStation->getId());

        graph.addBidirectionalEdge(origNode, destNode, capacity, service);
    }
    sortDistricts();
    sortMunicipalities();
    file.close();
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

int Database::getNumNodes() {
    return graph.getNumNodes();
}

void Database::sortDistricts() {
    sort(districts.begin(), districts.end());
}

void Database::sortMunicipalities() {
    for(auto &municipalities: municipalitiesByDistrict){
        sort(municipalities.second.begin(), municipalities.second.end());
    }

}

vector<pair<string, int>> Database::getTopMunicipies(int k) { //Soma todos os fluxos de cada municipio e ordena
    vector<pair<string, int>> res;
    for (auto &municipality: stationsByMunicipality) {
        int curFlow;
        graph.maxSomePairs(municipality.second, &curFlow);
        res.emplace_back(municipality.first, curFlow);
    }
    sort(res.begin(), res.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
        return a.second > b.second;
    });
    vector<pair<string, int>> topMunicipalities;
    for (int i = 0; i < k; i++) {
        topMunicipalities.push_back(res[i]);
    }
    return topMunicipalities;

    }

vector<pair<string, int>> Database::getTopDistricts(int k){
    vector<pair<string, int>> res;
    for (auto &district: municipalitiesByDistrict) {
        int curFlow;
        vector<int> stations;
        for(auto &municipality: district.second){
            for(auto &station: stationsByMunicipality[municipality]){
                stations.push_back(station);
            }
        }
        graph.maxSomePairs(stations, &curFlow);
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
