#include <iostream>
#include "Menu.h"

Menu::Menu() {
}
int Menu::main_menu(){
    while (true) {
        cout << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n"
                "|                     TSP Algorithms                           |\n"
                "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n"
                "[1] Backtracking Algorithm                                     |\n"
                "[2] Triangular Approximation                                   |\n"
                "[3] Christofides Algorithm                                     |\n"
                "[4] //TODO                                                     |\n"
                "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n"
                "|                       More ops                               |\n"
                "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n"
                "[5] Run all algorithms                                         |\n"
                "[6] Show all edges                                             |\n"
                "[7] Show all vertexes                                          |\n"
                "[8] Change files                                               |\n"
                "[9] Check current file(s)                                      |\n"
                "[0] Exit                                                       |\n"
                "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n";

        cout << endl;
        cout << "Please choose an option:";
        int option;
        cin >> option;
        cout << endl;
        if (cin.fail() || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cerr << "Invalid input" << endl;
            continue;
        }

        switch (option) {
            case 0:
                exit(0);
            case 1:
                graph.backtracking();
                break;
            case 2:
                graph.triangularApproximation();
                break;
            case 3:
                graph.Christofides();
                break;
            case 4:
                //TODO
                break;
            case 5:
                graph.runAllAlgorithms();
                break;
            case 6:
                graph.check_edges();
                break;
            case 7:
                graph.check_vertexes();
                break;
            case 8:
                change_to_other_file();
                break;
            case 9:
                graph.checkCurrentFiles();
                break;
            default:
                cerr << "Invalid input" << endl;
        }
    }
}
int Menu::run(){
     cout << "-------------------------------------------------------------------\n"
             "Welcome to Our TSP Algorithms Interface(default file = tourism.csv)\n"
             "-------------------------------------------------------------------\n";
    return main_menu();
}

void Menu::change_to_other_file() {
    string vertexes,edges;
    int input;
    cout<<"What combination of files do you want (type 1 for only edges,type 2 for vertex and edges,type 0 to go back)?" << endl;
    cin>>input;

    if (cin.fail() || cin.peek() != '\n') {
        cerr << "Invalid input" << endl;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        change_to_other_file();
    }

    switch (input) {
        case 1:
            cout<<"Enter the name of the edge file (without csv):\n";
            getline(cin>>ws,edges);
            cout<<endl;
            if(currently_used_file(edges + ".csv")){
                cerr << endl;
                cerr << "Either invalid input or file is being used" << endl;
                cout << "<< Returning to menu" << endl;
                break;
            }

            graph = Graph(edges + ".csv");
            cout << "Changed Successfully" << endl;
            main_menu();
            break;
        case 2:
            cout<<"Enter the name of the edge file (without csv):\n";
            getline(cin>>ws,edges);
            cout<<endl;
            cout<<"Enter the name of the vertex file (without csv):\n";
            getline(cin,vertexes);
            cout<<endl;
            if(currently_used_file(edges + ".csv") || currently_used_file(vertexes + ".csv")){
                cerr << endl;
                cerr << "Either invalid input or file(s) is/are being used" << endl;
                cout << "<< Returning to menu" << endl;
                break;
            }

            graph = Graph(edges + ".csv", vertexes + ".csv");
            cout << "Changed Successfully" << endl;
            main_menu();
            break;
        default:
            break;
    }
}


int Menu::currently_used_file(string file){
    ifstream  iss(file);
    if(file == graph.getCurrentEdgesFile() || file == graph.getCurrentVertexesFile() || !iss.is_open()) return 1;
    return 0;
}

