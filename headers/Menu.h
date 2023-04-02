#ifndef PROJECT_DA_MENU_H
#define PROJECT_DA_MENU_H

#include "Database.h"

class Menu {
    public:
        Menu(Database& database) : database(database) {};
        void showDataSelectionMenu();
        void showMainMenu();
        void showSubsetMenu();
        void showStationInfoMenu();
        void showTwoStationsInfoMenu();
        void showNetworkInfoMenu();

        set<string> getStringsFromUser();
        string getStringFromUser();
        int getIntFromUser();

    private:
        Database database;

};


#endif //PROJECT_DA_MENU_H
