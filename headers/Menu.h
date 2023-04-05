#ifndef PROJECT_DA_MENU_H
#define PROJECT_DA_MENU_H

#include "Database.h"
/**
 * @brief Class that contains all the menus used in the program.
 */
class Menu {
    public:
        Menu(Database& database) : database(database) {};
        void showDataSelectionMenu();
        void showMainMenu();
        void showSubsetMenu();
        void showStationInfoMenu();
        void showTwoStationsInfoMenu();
        void showNetworkInfoMenu();
        Station& selectStation();

        set<string> getStringsFromUser();
        string getStringFromUser();
        Station* getStationFromUser();
        Station* selectStationFromList();
        int getIntFromUser();
        void waitForInput();

    private:
        Database database;

    void showChangeCapacityMenu();
};


#endif //PROJECT_DA_MENU_H
