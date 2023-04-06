#include "headers/Database.h"
#include "headers/Menu.h"

/*
 * @brief Initializes Program
 */
int main(){
    Database database;
    Menu menu(database);
    menu.showDataSelectionMenu();
    return 0;
}