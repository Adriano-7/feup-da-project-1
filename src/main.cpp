#include "main.h"

int main(){
    Database database;
    Menu menu(database);
    menu.showInitialMenu();
    return 0;
}