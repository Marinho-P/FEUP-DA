#ifndef MENU_H
#define MENU_H

#include "Data.h"


/** @brief Class that serves as the interactive menu for the user */
class Menu {
private:
    /** @brief Attribute that can access all data processed */
    Data data;
public:
    Menu();
    int run();
private:
    /** @brief Exits the menu and displays an exit message */
    void exit();
    /**
     * @brief Displays the main menu options
     * @return The selected menu option.
     */
    int menuOptions() const;
    /**
     * @brief Displays the menu options for "1. List Basic Service Metrics" option
     * @return The selected menu option.
     */
    int toyGraphsOptions();

    void toyGraphsMethods();
};


#endif