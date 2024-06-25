#ifndef RESERVOIR_H
#define RESERVOIR_H

#include "NodeData.h"


/** @brief Class that stores information about reservoirs, including their name, municipality and max delivery. */
class Reservoir: public NodeData{
    private:
        string name;
        string municipality;
        int id;
        string code; // water reservoir code
        int maxDelivery; // m3/sec
public:
    /**
     * @brief Reservoir class constructor
     * This constructor calls the constructor of the base class NodeData to initialize a node of type RESERVOIR
     * @param name = name of the water reservoir
     * @param municipality = municipality where the water reservoir is located
     * @param id = unique identifier of water reservoir
     * @param code = code of water reservoir
     * @param maxDelivery = maximum delivery of water reservoir in m3/sec
     */
    Reservoir(const string &name, const string &municipality, const int &id, const string &code, const int &maxDelivery);
    const string &getName() const;
    const string &getMunicipality() const;
    int getMaxDelivery() const;
    bool operator==(const Reservoir& other) const;
};


#endif