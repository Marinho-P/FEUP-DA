#ifndef CITY_H
#define CITY_H

#include "NodeData.h"


/** @brief Class that stores information about cities, including their name, demand and population. */
class City : public NodeData{
private:
    string name;
    double demand;
    int population;
public:
    /**
     * @brief City class constructor
     * This constructor calls the constructor of the base class NodeData to initialize a node of type CITY
     * @param name = city of the delivery site
     * @param id = unique identifier of the delivery site
     * @param delivery_code = code of the delivery site
     * @param demand = rate demand or need of delivery site in m3/sec
     * @param population = total population of the delivery site
     */
    City(const string& name,const int& id,const string& delivery_code, const double& demand, const int& population);
    const string &getName() const;
    double getDemand() const;
    int getPopulation() const;
    bool operator==(const City& other) const;
};


#endif