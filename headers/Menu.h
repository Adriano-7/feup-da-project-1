#ifndef PROJECT_DA_MENU_H
#define PROJECT_DA_MENU_H

#include "Database.h"

/**
 * @brief Class responsible for the user interface
 */
class Menu {
    public:
        /**
         * @brief Class constructor of Menu that sets the database
         * @details Time complexity: O(1)
         * @param database - Database to be used
         */
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
};


#endif //PROJECT_DA_MENU_H
