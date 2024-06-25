#ifndef NODEDATA_H
#define NODEDATA_H

#include "string"

using namespace std;

enum nodeType{
    RESERVOIR,
    STATION,
    CITY,
};


/** @brief Class that stores nodes, be them cities, stations or reservoirs. */
class NodeData {
public:
    /**
     * @brief NodeData class constructor
     * This constructor initializes the different types of graph nodes in the same way
     * @param id = unique identifier of the node
     * @param code = code of the node
     * @param type = type of the node (RESERVOIR, STATION, CITY)
     */
    NodeData(int id, const string &code, nodeType type);
    nodeType getType() const;
    int getId() const;
    const string &getCode() const;
    bool operator==(const NodeData& other) const;
    virtual ~NodeData() = default;
protected:
    int id;
    string code;
    nodeType type;
};


#endif