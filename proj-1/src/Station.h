#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <string>
#include "NodeData.h"

using namespace std;


/** @brief Class that stores information about stations. */
class Station: public NodeData{
public:
    /**
     * @brief Station class constructor
     * This constructor calls the constructor of the base class NodeData to initialize a node of type STATION
     * @param id = unique identifier of pumping station
     * @param code = code of pumping station
     */
    Station(int id, string code);
    bool operator==(const Station& other) const;
};


#endif