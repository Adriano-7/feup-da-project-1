#include "Menu.h"
#include <iostream>
#include <limits>

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

    int option;
    cin >> option;
    if (cin.fail()) {
        cout << "Invalid input" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        showSubsetMenu();
    }
    else {
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
    }
    database.loadWithFilters(stations, lines);
    showMainMenu();
}

void Menu::showMainMenu(){
    cout << "_________________________________________________" << endl;
    cout << "Please select an option:" << endl;
    cout << "1 - See information about a single station" << endl;
    cout << "2 - See information about two stations" << endl;
    cout << "3 - See information about the entire network" << endl;
    cout << "4 - Return to data selection menu" << endl;
    cout << "5 - exit" << endl;

    int option = getIntFromUser();
    switch (option) {
        case 1:
            showStationInfoMenu();
            break;
        case 2:
            showTwoStationsInfoMenu();
            break;
        case 3:
            showNetworkInfoMenu();
            break;
        case 4:
            showDataSelectionMenu();
            break;
        case 5:
            exit(0);
        default:
            cout << "Invalid option" << endl;
            showMainMenu();
    }
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
    getline(cin, input);
    if (input.empty()) {
        cout << "Invalid input" << endl;
        return getStringFromUser();
    }
    return input;
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

void Menu::showStationInfoMenu() {
    cout << "_________________________________________________" << endl;
    cout << "Please enter the name of the station:" << endl;
    string stationName = getStringFromUser();

    Station *station = database.getStation(stationName);
    if (station == nullptr) {
        cout << "Station not found" << endl;
        showStationInfoMenu();
    }
    else {
        cout << "_________________________________________________" << endl;
        cout << "Station name: " << station->getName() << endl;
        cout << "District: " << station->getDistrict() << endl;
        cout << "Municipality: " << station->getMunicipality() << endl;
        cout << "Township: " << station->getTownship() << endl;
        cout << "Line: " << station->getLine() << endl;
        cout << "_________________________________________________" << endl;
    }
}

void Menu::showTwoStationsInfoMenu() {
    cout << "_________________________________________________" << endl;
    cout << "Please enter the name of the first station:" << endl;
    string stationName1=getStringFromUser();
    Station* station1 = database.getStation(stationName1);
    if (station1 == nullptr) {
        cout << "Station not found" << endl;
        showTwoStationsInfoMenu();
    }

    cout << "Please enter the name of the second station:" << endl;
    string stationName2=getStringFromUser();
    Station* station2 = database.getStation(stationName2);
    if (station2 == nullptr) {
        cout << "Station not found" << endl;
        showTwoStationsInfoMenu();
    }

    int flow = database.getMaxFlowBetweenStations(stationName1, stationName2);
    cout << "_________________________________________________" << endl;
    cout << "Station 1: " << stationName1 << endl;
    cout << "Station 2: " << stationName2 << endl;
    cout << "The maximum number of trains that can pass between the two stations is: " << flow << endl;

}

void Menu::showNetworkInfoMenu() {
    cout << "_________________________________________________" << endl;
    cout << "Not implemented yet" << endl;
}
