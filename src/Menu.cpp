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
    ServiceType service = ServiceType::NONE;
    string serviceType = "";
    bool isFiltering = true;

    while (isFiltering) {
        cout << "_________________________________________________" << endl;
        cout << "Please select an option:" << endl;

        if(!stations.empty() || !lines.empty() || service != ServiceType::NONE){
            cout << "Current filters:" << endl;
            for(const auto& station : stations)
                cout << "   Station: " << station << endl;
            for(const auto& line : lines)
                cout << "   Line: " << line << endl;
            if(service == ServiceType::STANDARD)
                cout << "   Service: Standard" << endl;
            else if(service == ServiceType::ALFA_PENDULAR)
                cout << "   Service: ALFA PENDULAR" << endl;
        }

        cout << "1 - Filter by lines" << endl;
        cout << "2 - Filter by railway stations" << endl;
        cout << "3 - Filter by service" << endl;
        cout << "4 - Load network" << endl;

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
                    lines = getStringsFromInput();
                    break;
                case 2:
                    stations = getStringsFromInput();
                    break;
                case 3:
                    serviceType = (*getStringsFromInput().begin());
                    if(serviceType == "Standard" || serviceType == "STANDARD" || serviceType == "standard")
                        service = STANDARD;
                    else if(serviceType == "ALFA PENDULAR" || serviceType == "Alfa Pendular" || serviceType == "alfa pendular")
                        service = ALFA_PENDULAR;
                    else
                        cout << "Invalid service type" << endl;
                    break;
                case 4:
                    database.loadWithFilters(stations, lines, service);
                    isFiltering = false;
                    showMainMenu();
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
            }
        }
    }
}

void Menu::showMainMenu(){

        printf("_________________________________________________\n");
        database.printNodes();

}

set<string> Menu::getStringsFromInput() {
    set<string> strings;
    string input;
    cout << "Please enter the strings, one per line. Enter 0 to finish." << endl;

    while (true) {
        getline(cin, input);
        if (input == "0")
            return strings;
        else if (input.empty()) {
            cout << "Invalid input" << endl;
        }
        else {
            strings.insert(input);
        }
    }
}