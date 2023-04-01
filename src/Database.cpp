#include "headers/Database.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Database::Database() {
    readStations();
    readNetwork();
}

void Database::readStations() {
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

        Station *station = new Station(name, district, municipality, township, line);
        graph.addNode(*station);
        nameToStation[name] = station;
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
    while (getline(file, line)) {
        vector<string> fields;
        istringstream stream(line);
        string field;
        while (getline(stream, field, ';')) {
            fields.push_back(field);
        }
        if (fields.size() != 4) {
            cout << "The following line is invalid: " << line << endl;
            return;
        }

        string orig = fields[0];
        string dest = fields[1];
        int capacity = stoi(fields[2]);
        ServiceType service = (fields[3] == "STANDARD") ? STANDARD : ALFA_PENDULAR;

        Station *origStation = nameToStation[orig];
        Station *destStation = nameToStation[dest];

        if (origStation == nullptr){
            cout << "The following station wasn't initialized: " << orig << endl;
            return;
        }
        if (destStation == nullptr){
            cout << "The following station wasn't initialized: " << dest << endl;
            return;
        }

        graph.addEdge(*origStation, *destStation, capacity, service);
    }
    file.close();
    return;
}