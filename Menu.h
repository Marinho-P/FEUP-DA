#ifndef MENU_H
#define MENU_H
#include "Graph.h"

/**
 * @brief Class that serves as the interactive menu for the user.
 */
class Menu {
    private:
        Graph graph = Graph ("tourism.csv");
    public:
        /**
         * @brief Constructor for the Menu class.
         */
        Menu();

        /**
         * @brief Displays the main menu and handles user input.
         * @return An integer representing the user's choice.
         */
        int run();

        /**
        * @brief Checks if a given file is currently being used.
        * @param file The name of the file to check.
        * @return An integer indicating whether the file is currently used (1) or not (0).
        */
        int currently_used_file(string file);

        /**
         * @brief Changes the current file(s) being used by the graph.
         */
        void change_to_other_file();

        /**
         * @brief Displays the main menu and handles user input.
         * @return An integer representing the user's choice.
         */
        int main_menu();
};

#endif