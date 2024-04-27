#ifndef DATA_H
#define DATA_H

#define SHIPPING "../Toy-Graphs/shipping.csv"
#define STADIUMS "../Toy-Graphs/stadiums.csv"
#define TOURISM "../Toy-Graphs/tourism.csv"



#include <unordered_map>
#include "data_structures/Graph.h"
#include <unordered_set>
using namespace std;


/** @brief Class where all classes interact and where system requests are handled */
class Data {
public:
    /** @brief Constructor for the Data class */
    Data();
    void readToyGraph(string inputCSV);
    void verifyGraph();
private:
    Graph<int>* graph;
};


#endif