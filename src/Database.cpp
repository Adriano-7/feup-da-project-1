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

/**
 * @brief Reads csv files and populates the data structures (The arguments are filters, if empty, all stations are read)
 * @param stations - set of stations to read (if empty, all stations are read)
 * @param lines - set of railway lines to read (if empty, all lines are read)
 * @details Time complexity: O(A) + O(B), where A is the number of lines in stations.csv and B is the number of lines in network.csv
*/
void Database::loadWithFilters(set<string> stations, set<string> lines) {
    readStations(stations, lines);
    readNetwork();
}

/**
 * @brief Given a station name, returns a pointer to the station.
 * @details Time complexity: O(1)
 * @param stationName - Name of the station to be returned.
 * @return Pointer to the station.
 */
Station* Database::getStation(string stationName) {
    return nameToStation[stationName];
}

/**
 * @brief Returns the pairs of nodes with the maximum flow between them and the value of the maximum flow(by reference).
 * @details Time Complexity: O(V^3E^2), where E is the number of edges and V is the number of nodes.
 * @param maxFlow - Pointer to the variable that will store the value of the maximum flow.
 * @return Vector of pairs of nodes with the maximum flow between them.
 */
vector<pair<Node *, Node *>> Database::maxFlowAllPairs(int *maxFlow){
    return graph.maxFlowAllPairs(maxFlow);
}

/**
 * @brief Returns the map that associates each district to the set of municipalities in that district.
 * @details Time complexity: O(1)
 * @return Map that associates each district to the set of municipalities in that district.
 */
map<string, set<string>> Database::getDistrictToMunicipalities(){
    return districtToMunicipalities;
}

/**
 * @brief Returns the set of stations in a given municipality.
 * @details Time complexity: O(1)
 * @param municipality - municipality to check
 * @return Set of stations in the given municipality.
*/
set<string> Database::getStationsFromMunicipality(string municipality){
    return municipalityToStations[municipality];
}

/**
 * @brief Using Edmonds-Karp algorithm, calculates the maximum flow between two stations.
 * @details Time Complexity: O(VE^2), where E is the number of edges and V is the number of nodes.
 * @param station1 - source station
 * @param station2 - destination station
 * @return maximum flow between the two stations
 */
int Database::getMaxFlowBetweenStations(Station* station1, Station* station2) {
    return graph.EdmondsKarp(graph.getNode(station1), graph.getNode(station2));
}

/**
 * @brief Using dijkstra algorithm, calculates the minimum cost(cost of service * capacity) flow between two stations
 * @details Time Complexity: O((V+E log V), where E is the number of edges and V is the number of nodes.
 * @param station1 - source station
 * @param station2 - destination station
 * @param flow - pointer to the variable that will store the value of the flow
 * @param costService - pointer to the variable that will store the value of the cost of service
 * @return stack of edges of the path with the minimum cost
*/
stack<Edge*> Database::getMinCostFlow(Station* station1, Station* station2, double *flow, double *costService){
    return graph.FordFulkersonDijkstra(graph.getNode(station1), graph.getNode(station2), flow, costService);
}

/**
 * @brief Returns the maximum number of trains that can be in a station at the same time.
 * @details Time Complexity: O(V E^2), where E is the number of edges and V is the number of nodes.
 * @param station - station to check
 * @return maximum number of trains that can be in the station at the same time
*/
int Database::getMaxTrainsStation(Station* station){
    return graph.maxIncomingFlow(graph.getNode(station));
}

/**
 * @brief Calculates the maximum number of trains that can be in a station at the same time for all stations.
*/
void Database::maxTrainAllStations(){
    for(auto nodePair: nameToStation){
        Node* node = graph.getNode(nodePair.second);
        stationToNumTrains[node->getStationName()].push_back(graph.maxIncomingFlow(node));
    }
}

/**
 * @brief Get the top "k" municipalities based on the sum of the flow values of all of their stations.
 * @details Time Complexity: O(VE^2P^2*M), where E is the number of edges, V is the number of nodes in the graph, P is the number of nodes in the input set and M is the number of municipalities.
 * @param k - number of municipalities to return
 * @return vector of pairs of municipality name and total flow
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
 * @brief Get the top "k" districts based on the sum of the flow values of all of their stations.
 * @details Time Complexity: O(VE^2P^2*D), where E is the number of edges, V is the number of nodes in the graph, P is the number of nodes in the input set and D is the number of districts.
 * @param k - number of districts to return
 * @return vector of pairs of district name and total flow
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
 * @brief Reads stations.csv and creates the nodes of the graph.
 * @details Time Complexity: O(A), where A is the number of lines in stations.csv
 * @param stations - set of stations to read (if empty, all stations are read)
 * @param lines - set of railway lines to read (if empty, all lines are read)
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
 * @brief Reads network.csv and creates the edges of the graph.
 * @details Time Complexity: O(B), where B is the number of lines in network.csv
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
 * @brief Boolean function that checks if there is a direct connection between two stations.
 * @details Time Complexity: O(E), where E is the number of adjacent edges.
 * @param station1 - pointer to the first station
 * @param station2 - pointer to the second station
 * @param edgeCapacity - reference to an integer that will be filled with the capacity of the edge
 * @return true if there is a direct connection, false otherwise
 */
bool Database::checkConnection(Station* station1, Station* station2, int& curCapacity) {
    return graph.checkConnection(graph.getNode(station1), graph.getNode(station2), curCapacity);
}

/**
 * @brief Changes the capacity of an edge between two stations.
 * @details Time Complexity: O(E), where E is the number of adjacent edges.
 * @param station1 - pointer to the first station
 * @param station2 - pointer to the second station
 * @param newCapacity - new capacity of the edge
 */
void Database::changeCapacity(Station* station1, Station* station2, int newCapacity) {
    graph.changeCapacity(graph.getNode(station1), graph.getNode(station2), newCapacity);
}

/**
 * @brief Finds the shortest path between two stations.
 */
map<string, vector<int>> Database::getStationToNumTrains() {
    return stationToNumTrains;
}
