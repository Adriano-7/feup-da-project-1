#include "headers/Database.h"
#include "headers/Menu.h"
/**
 * @brief Initializes Program
 * @details Time Complexity: O(1) Initializes the database and the menu.
 * @return
 */
int main(){
    Database database;
    Menu menu(database);
    menu.showDataSelectionMenu();
    return 0;
}