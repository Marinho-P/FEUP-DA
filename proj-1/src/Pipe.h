#ifndef PIPE_H
#define PIPE_H

#include "string"

using namespace std;


/** @brief Class that stores information about pipes, including their source and target service point, capacity and direction. */
class Pipe {
    private:
        string servicePointA;
        string servicePointB;
        int capacity;
        bool direction;
public:
    /**
     * @brief Pipe class constructor
     * @param servicePointA = source service that can be a water reservoir, a pumping station, or a delivery site
     * @param servicePointB = target service that can be a water reservoir, a pumping station, or a delivery site
     * @param capacity = maximum capacity of the pipe
     * @param direction = boolean indicating whether the connection between source service and target service is undirected (1 - unidirectional and 0 - bidirectional)
     */
    Pipe(const string &servicePointA, const string &servicePointB, int capacity, bool direction);
    const string &getServicePointA() const;
    const string &getServicePointB() const;
    int getCapacity() const;
    bool isDirection() const;
};


#endif