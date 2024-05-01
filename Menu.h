#ifndef MENU_H
#define MENU_H
#include "Graph.h"

/** @brief Class that serves as the interactive menu for the user */
class Menu {
private:
    Graph graph = Graph ("tourism.csv");
public:
    Menu();
    int run();

    int currently_used_file(string file);

    void change_to_other_file();

    int main_menu();
};


#endif