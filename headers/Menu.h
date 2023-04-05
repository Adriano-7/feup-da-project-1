#ifndef PROJECT_DA_MENU_H
#define PROJECT_DA_MENU_H

#include "Database.h"

class Menu {
    public:
        Menu(Database& database) : database(database) {};
        void showDataSelectionMenu();
    private:
        Database database;

        void showSubsetMenu();
        void showMainMenu();
        void showStationInfoMenu();
        void showTwoStationsInfoMenu();
        void showNetworkInfoMenu();
        void showChangeCapacityMenu();

        set<string> getStringsFromUser();
        string getStringFromUser();
        Station* getStationFromUser();
        Station* selectStationFromList();
        int getIntFromUser();
        string serviceToString(ServiceType service);

        void waitForInput();

    void printReport();
};


#endif //PROJECT_DA_MENU_H
