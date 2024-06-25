#include "Edge.h"

int Edge::getDestiny() const {
    return destiny;
}


double Edge::getDistance() const {
    return distance;
}


Edge::Edge(int destiny, double distance) : destiny(destiny), distance(distance) {}