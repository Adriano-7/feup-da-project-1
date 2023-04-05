#include "headers/Database.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief Reads data from the csv files whitout filters.
 * @details Time complexity: O()
 */
void Database::loadWithoutFilters() {
    readStations({}, {});
    readNetwork();
}

/*
 * @brief Reads data from the csv files with filters (stations or lines).
 * @details Time complexity: O()
*/
void Database::loadWithFilters(set<string> stations, set<string> lines) {
    readStations(stations, lines);
    readNetwork();
}

/*
 * @brief Given a station name, returns a pointer to the station.
 * @details Time complexity: O(1)
 * */
Station* Database::getStation(string stationName) {
    return nameToStation[stationName];
}

/*
 * @brief Calculates the maximum flow between all pairs of stations.
 * @details Time complexity: O()
 * */
vector<pair<Node *, Node *>> Database::maxFlowAllPairs(int *maxFlow){
    return graph.maxFlowAllPairs(maxFlow);
}

/*
 * @brief Gives the map of districts to municipalities.
 * @details Time complexity: O(1)
 * */
map<string, set<string>> Database::getDistrictToMunicipalities(){
    return districtToMunicipalities;
}

/*
 * @brief Given the municipality, returns the set of stations in that municipality.
 * @details Time complexity: O(1)
 * */
set<string> Database::getStationsFromMunicipality(string municipality){
    return municipalityToStations[municipality];
}

/*
 * @brief Using Edmonds-Karp algorithm, calculates the maximum flow between two stations.
 * */
int Database::getMaxFlowBetweenStations(Station* station1, Station* station2) {
    return graph.EdmondsKarp(graph.getNode(station1), graph.getNode(station2));
}

/*
 * @brief Using dijkstra algorithm, calculates the minimum cost(cost of service * capacity) flow between two stations
 * @details Time complexity: O()
 * @param station1 - source station
 * @param station2 - destination station
*/
stack<Edge*> Database::getMinCostFlow(Station* station1, Station* station2, double *flow, double *costService){
    return graph.FordFulkersonDijkstra(graph.getNode(station1), graph.getNode(station2), flow, costService);
}

int Database::getMaxTrainsStation(Station* station){
    return graph.maxIncomingFlow(graph.getNode(station));
}
/**
 * @brief Get the top "k" municipalities based on total flow.
 * @details Time Complexity: O(m(slog(sz)+e) Returns a vector of the top k municipalities based on their total flow (sum of flow values) from all of their stations in a graph.
 * @param k
 * @return
 */
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
/**
 * @brief Get the top "k" districts based on total flow.
 * @details Time Complexity: O(N*M) Calculates the maximum flow through sets of stations in each district, and returns a vector of the top k districts based on this flow metric.
 * @param k
 * @return
 */
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
/**
 * @brief Reads data from the csv files.
 * @details Time Complexity: O(N*(n+m)) Reads data from a file named stations.csv and creates Station objects based on the data. The Station objects are added to a graph, which is likely a data structure used to represent a network of stations.
 * @param stations
 * @param lines
 */
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
/**
 * @brief Reads data from the csv files.
 * @details Time Complexity: O(n) Reads network data from a CSV file located at "../data/network.csv" and stores it in a graph data structure.
 */
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

        graph.addBidirectionalEdge(graph.getNode(orig), graph.getNode(dest), capacity, service);
    }
    file.close();
    return;
}
/**
 * @brief Checks the connection between stations.
 * @details Time Complexity: O() Passes on the arguments to the corresponding checkConnection function of the graph object, and returns its boolean result.
 * @param station1
 * @param station2
 * @param curCapacity
 * @return
 */
bool Database::checkConnection(Station* station1, Station* station2, int& curCapacity) {
    return graph.checkConnection(graph.getNode(station1), graph.getNode(station2), curCapacity);
}
/**
 * @brief Changes capacity from an edge.
 * @details Time Complexity: O() Passes on the arguments to the corresponding changeCapacity function of the graph object, and returns its boolean result.
 * @param station1
 * @param station2
 * @param newCapacity
 */
void Database::changeCapacity(Station* station1, Station* station2, int newCapacity) {
    return graph.changeCapacity(graph.getNode(station1), graph.getNode(station2), newCapacity);
}
