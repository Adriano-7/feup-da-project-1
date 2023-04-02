#ifndef PROJECT_DA_MENU_H
#define PROJECT_DA_MENU_H

#include "Database.h"

class Menu {
    public:
        Menu(Database& database) : database(database) {};
        void showDataSelectionMenu();
        void showMainMenu();
        void showSubsetMenu();

        set<string> getStringsFromInput();

    private:
        Database database;

};


#endif //PROJECT_DA_MENU_H
