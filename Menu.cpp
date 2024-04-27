#include <iostream>
#include "Menu.h"

Menu::Menu() {
}

int Menu::run(){
    int option = menuOptions();
    while(option != 0){
        switch(option){
            case 1:
                toyGraphsMethods();
                break;
            case 2:
                //TODO
                break;
            default:
                std::cout << "Invalid option try again" << std::endl;
                cout << "\n";
        }
        option = menuOptions();
    }
    exit();
    return 0;
}


void Menu::exit(){
    std::cout << ">> Exiting the menu." << std::endl;
}


int Menu::menuOptions() const {
    int option;
    std::cout << "\n";
    std::cout << "1. See toy graphs" << std::endl;
    std::cout << "2. See Real World graphs" << std::endl;
    std::cout << "3. See Extra Fully Connected Graphs" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your option:";
    std::cin >> option;
    return option;
}

int Menu::toyGraphsOptions(){
    int option;
    cout << "\n";
    std::cout << "1. Check shipping graph methods" << std::endl;
    std::cout << "2. Check stadiums graph methods" << std::endl;
    std::cout << "3. Check tourism graph methods" << std::endl;
    std::cout << "0. Back to main menu" << std::endl;
    std::cout << "Enter your option:";
    std::cin >> option;
    return option;
}

void Menu::toyGraphsMethods() {
    int option = toyGraphsOptions();
    while (option){
        switch (option) {
            case 1:
                data.readToyGraph(SHIPPING);
                data.verifyGraph();
                //TODO
                break;
            case 2:
                data.readToyGraph(STADIUMS);
                //TODO
                data.verifyGraph();
                break;
            case 3:
                data.readToyGraph(TOURISM);
                //TODO
                data.verifyGraph();
                break;
            default:
                std::cout << "Invalid option try again" << std::endl;
                cout << "\n";
        }
        option = toyGraphsOptions();
    }
}