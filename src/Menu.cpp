#include "Menu.h"
#include <iostream>
#include <limits>
#include <unistd.h>
#include <algorithm>

void Menu::showDataSelectionMenu() {
    cout << "_________________________________________________" << endl;
    cout << "Welcome to the railway network management system!" << endl;
    cout << "Please select an option:" << endl;
    cout << "1 - Load the entire network" << endl;
    cout << "2 - Load a subset of the network" << endl;

    int option;
    cin >> option;
    if (cin.fail()) {
        cout << "Invalid input" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        showDataSelectionMenu();
    }
    else {
        switch (option) {
            case 1:
                database.loadWithoutFilters();
                showMainMenu();
                break;
            case 2:
                showSubsetMenu();
                break;
            default:
                cout << "Invalid option" << endl;
                showDataSelectionMenu();
        }
    }
}

void Menu::showSubsetMenu() {
    set<string> stations;
    set<string> lines;

    cout << "_________________________________________________" << endl;
    cout << "Please select an option:" << endl;
    cout << "1 - Filter by lines" << endl;
    cout << "2 - Filter by railway stations" << endl;

    int option = getIntFromUser();

    switch (option) {
        case 1:
            lines = getStringsFromUser();
            break;
        case 2:
            stations = getStringsFromUser();
            break;
        default:
            cout << "Invalid option" << endl;
            break;
    }

    database.loadWithFilters(stations, lines);
    showMainMenu();
}

void Menu::showMainMenu(){
    while (true){
        cout << "_________________________________________________" << endl;
        cout << "Please select an option:" << endl;
        cout << "1 - See information about a single station" << endl;
        cout << "2 - See information about two stations" << endl;
        cout << "3 - See information about the entire network" << endl;
        cout << "4 - Make a change to the capacity of a connection" << endl;
        cout << "5 - Exit" << endl;

        int option = getIntFromUser();
        switch (option) {
            case 1:
                showStationInfoMenu();
                waitForInput();
                break;
            case 2:
                showTwoStationsInfoMenu();
                waitForInput();
                break;
            case 3:
                showNetworkInfoMenu();
                waitForInput();
                break;
            case 4:
                showChangeCapacityMenu();
                waitForInput();
                break;
            case 5:
                return;
            default:
                cout << "Invalid option" << endl;
        }
    }
}

void Menu::showStationInfoMenu() {
    Station* station = getStationFromUser();
    if (station == nullptr) {
        cout << "Station not found" << endl;
        return showStationInfoMenu();
    }

    cout << "_________________________________________________" << endl;
    cout<< "Please select an option:" << endl;
    cout<< "1 - See the maximum number of trains that can arrive to the station" << endl;
    cout<< "2 - See more information about the station" << endl;
    cout<< "3 - Return to main menu" << endl;

    int option = getIntFromUser();
    int maxTrains;

    switch(option){
        case 1:
            maxTrains = database.getMaxTrainsStation(station);
            cout << "_________________________________________________" << endl;
            cout << "The maximum number of trains that can arrive to the station is: " << maxTrains << endl;
            break;
        case 2:
            cout << "_________________________________________________" << endl;
            cout << "Station name: " << station->getName() << endl;
            cout << "District: " << station->getDistrict() << endl;
            cout << "Municipality: " << station->getMunicipality() << endl;
            cout << "Township: " << station->getTownship() << endl;
            cout << "Line: " << station->getLine() << endl;
            cout << "_________________________________________________" << endl;
            break;
        case 3:
            return;
        default:
            cout << "Invalid option" << endl;
            showStationInfoMenu();
    }
}

void Menu::showTwoStationsInfoMenu() {
    cout << "_________________________________________________" << endl;
    cout << "First station:" << endl;
    Station* station1 = getStationFromUser();

    if (station1 == nullptr) {
        cout << "Station not found" << endl;
        return showTwoStationsInfoMenu();
    }

    cout << "_________________________________________________" << endl;
    cout << "Second station:" << endl;
    Station* station2 = getStationFromUser();

    if (station2 == nullptr) {
        cout << "Station not found" << endl;
        return showTwoStationsInfoMenu();
    }

    cout << "_________________________________________________" << endl;
    cout<< "Please select an option:" << endl;
    cout<< "1 - Maximum number of trains that can travel between the two" << endl;
    cout<< "2 - Maximum number of trains that can travel between the two taking cost into account" << endl;
    cout<< "3 - Return to main menu" << endl;

    int option = getIntFromUser();
    double flow, cost;
    stack<Edge*> pathFlow;
    switch(option){
        case 1:
            flow = database.getMaxFlowBetweenStations(station1, station2);
            cout << "_________________________________________________" << endl;
            cout << "Station 1: " << station1->getName() << endl;
            cout << "Station 2: " << station2->getName() << endl;
            cout << "The maximum number of trains that can pass between the two stations is: " << flow << endl;
            break;
        case 2:
            pathFlow = database.getMinCostFlow(station1, station2, &flow, &cost);
            if(pathFlow.empty()){
                cout << "There is no path between " << station1->getName() << " and " << station2->getName() << endl;
                return;
            }

            cout << "_________________________________________________" << endl;
            cout << "Station 1: " << station1->getName() << endl;
            cout << "Station 2: " << station2->getName() << endl;
            cout << "The maximum number of trains that can pass between the two stations is: " << flow << endl;
            cout << "The cost of service is: " << cost << endl;
            cout << "The path is: " << endl;

            while(!pathFlow.empty()){
                Edge* edge = pathFlow.top();
                pathFlow.pop();
                cout << edge->getOrig()->getStationName() << " -> " << edge->getDest()->getStationName() << " | " << serviceToString(edge->getService()) << endl;
            }
            cout << endl;
            break;
        case 3:
            return;
        default:
            cout << "Invalid option" << endl;
            showTwoStationsInfoMenu();
    }
}

void Menu::showNetworkInfoMenu() {
    cout << "_________________________________________________" << endl;
    cout<< "Please select an option:" << endl;
    cout<< "1 - Pairs of stations with the maximum number of trains that can travel between them" << endl;
    cout<< "2 - Top-k municipalities and districts" << endl;
    cout<< "3 - Return to main menu" << endl;

    int option = getIntFromUser();
    int maxFlow;

    vector<pair<Node*, Node*>> pairs;
    vector<pair<string, int>> topK;
    int k;

    switch(option){
        case 1:
            pairs = database.maxFlowAllPairs(&maxFlow);
            cout << "The max flow is " << maxFlow << endl;

            for(auto pair: pairs){
                cout << pair.first->getStationName() << " -> " << pair.second->getStationName() << endl;
            }
            break;
        case 2:
            cout << "1 -Districts" << endl;
            cout << "2 -Municipalities" << endl;
            option = getIntFromUser();

            cout << "Enter the k number of top districts/municipalities" << endl;
            k = getIntFromUser();

            if(option == 1){
                topK = database.getTopDistricts(k);
            }
            else if(option == 2){
                topK = database.getTopMunicipalities(k);
            }
            else{
                cout << "Invalid option" << endl;
                showNetworkInfoMenu();
            }

            for(auto pair: topK){
                cout << pair.first << " -> " << pair.second << endl;
            }
            break;
        case 3:
            return;
        default:
            cout << "Invalid option" << endl;
            showNetworkInfoMenu();
    }
}

void Menu::showChangeCapacityMenu() {
    cout << "_________________________________________________" << endl;
    cout << "Do you wish to receive a report of the changes made? (y/n)" << endl;
    string s = getStringFromUser();

    bool report = false;
    if(s == "y"){
        report = true;
    } else if(s == "n"){
        report = false;
    } else {
        cout << "Invalid option" << endl;
        showChangeCapacityMenu();
    }

    //Chamamos a thread
    if(report) database.maxTrainAllStations();

    cout << "How many changes do you wish to make :" << endl;
    int nChanges = getIntFromUser();

    for (int i = 0; i < nChanges; i++) {
        cout << "_________________________________________________" << endl;
        cout << "Change " << i+1 << ":" << endl;
        cout << "Please enter the name of the first station:" << endl;
        Station* station1 = getStationFromUser();

        cout << "_________________________________________________" << endl;
        cout << "Please enter the name of the second station:" << endl;
        Station* station2 = getStationFromUser();

        int curCapacity;
        if(!database.checkConnection(station1, station2, curCapacity)){
            cout << "There is no path between the two stations" << endl;
            showMainMenu();
            return;
        }
        int newCapacity;

        while(true){
            cout << "Please enter the new capacity. It must be equal or less than " << curCapacity << endl;

            newCapacity = getIntFromUser();
            if(newCapacity > curCapacity){
                cout << "The new capacity must be less than " << curCapacity << endl;
            } else { break; }
        }
        database.changeCapacity(station1, station2, newCapacity);
        cout << "_________________________________________________" << endl;
        cout << "The capacity was changed" << endl;
    }

    if(report) {
        database.maxTrainAllStations();
        printReport();
    }

    cout << "If you wish to undo your changes you must restart the program." << endl;
}

set<string> Menu::getStringsFromUser() {
    set<string> strings;
    string input;
    bool firstTime = true;
    cout << "Please enter the strings, one per line. Enter 0 to finish." << endl;

    while (true) {
        getline(cin, input);
        if (input == "0")
            return strings;
        else if (input.empty() && !firstTime) {
            cout << "Invalid input" << endl;
        }
        else {
            strings.insert(input);
        }
        firstTime = false;
    }
}

string Menu::getStringFromUser() {
    string input;
    while (input.empty()) {
        getline(cin, input);
    }

    return input;
}

Station* Menu::getStationFromUser(){
    cout << "Please select an option" << endl;
    cout << "1 - Write the name of the station" << endl;
    cout << "2 - Select from a list of stations" << endl;

    int option = getIntFromUser();

    Station* station;
    switch(option){
        case 1:
            cout << "Please enter the name of the station" << endl;
            station = database.getStation(getStringFromUser());
            while (station== nullptr){
                cout << "Station not found, please try again" << endl;
                station = database.getStation(getStringFromUser());
            }
            break;
        case 2:
            cout << "Please select a station from the list" << endl;
            station = selectStationFromList();
            while (station== nullptr){
                cout << "Station not found, please try again" << endl;
                station = selectStationFromList();
            }
            break;
        default:
            cout << "Invalid option" << endl;
            return getStationFromUser();
    }
    return station;
}

Station* Menu::selectStationFromList(){
    cout<< "_________________________________________________" << endl;
    cout<< "Select the District:" << endl;

    map<string, set<string>> districtMunicipalities = database.getDistrictToMunicipalities();
    auto it = districtMunicipalities.begin();
    int i = 1;
    for(; it != districtMunicipalities.end(); it++){
        cout << i << " - " << it->first << endl;
        i++;
    }

    int option = -1;
    while(option < 1 || option > districtMunicipalities.size()){
        cout << "Please select an option" << endl;
        option = getIntFromUser();
    }

    it = districtMunicipalities.begin();
    advance(it, option-1);
    string district = it->first;

    cout<< "_________________________________________________" << endl;
    cout<< "Select the Municipality:" << endl;
    set<string> municipalities = it->second;

    auto it2 = municipalities.begin();
    i = 1;
    for(; it2 != municipalities.end(); it2++){
        cout << i << " - " << *it2 << endl;
        i++;
    }

    option = -1;
    while(option < 1 || option > municipalities.size()){
        cout << "Please select an option" << endl;
        option = getIntFromUser();
    }

    it2 = municipalities.begin();
    advance(it2, option-1);

    cout<< "_________________________________________________" << endl;
    cout<< "Select the Station:" << endl;
    set<string> stations = database.getStationsFromMunicipality(*it2);

    auto it3 = stations.begin();
    i = 1;
    for(; it3 != stations.end(); it3++){
        cout << i << " - " << *it3 << endl;
        i++;
    }

    option = -1;
    while(option < 1 || option > stations.size()){
        cout << "Please select an option" << endl;
        option = getIntFromUser();
    }

    it3 = stations.begin();
    advance(it3, option-1);

    return database.getStation(*it3);
}

int Menu::getIntFromUser() {
    int input;
    cin >> input;
    if (cin.fail()) {
        cout << "Invalid input" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return getIntFromUser();
    }
    return input;
}

string Menu::serviceToString(ServiceType service){
    switch(service){
        case ServiceType::STANDARD:
            return "Standard";
        case ServiceType::ALFA_PENDULAR:
            return "Alfa Pendular";
        default:
            return "Invalid Service";
    }
}

void Menu::waitForInput() {
    usleep(800000);
    string q;
    cout << endl << "Insert any key to continue: ";
    cin >> q;
    cout << endl;
}

void Menu::printReport() {
    int k;
    cout << "Please enter the number of stations you wish to see" << endl;
    k = getIntFromUser();
    while(k < 0){
        cout << "The number of stations must be positive" << endl;
        k = getIntFromUser();
    }

    vector<pair<string, int>> stationToDifference;

    auto stationToNumTrains = database.getStationToNumTrains();
    for(auto something: stationToNumTrains){
        if(something.second.size() != 2){
            //print the station
            cout << something.first << endl;

            cout << "Found the problem " << endl;
            return;

        }
        auto h = make_pair(something.first, something.second[1] - something.second[0]);
        stationToDifference.push_back(h);
    }
    //Sort stationToDifference by the second element of the pair
    sort(stationToDifference.begin(), stationToDifference.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    cout << "_________________________________________________" << endl;
    cout << "The " << k << " stations with the less trains are:" << endl;
    for(int i = 0; i < k; i++){
        cout << stationToDifference[i].first << " with " << stationToDifference[i].second << " less trains" << endl;
    }
}