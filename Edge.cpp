#include "Edge.h"

int Edge::getDestiny() const {
    return destiny;
}

void Edge::setDestiny(int destiny) {
    Edge::destiny = destiny;
}

double Edge::getDistance() const {
    return distance;
}

void Edge::setDistance(double distance) {
    Edge::distance = distance;
}

Edge::Edge(int destiny, double distance) : destiny(destiny), distance(distance) {}
