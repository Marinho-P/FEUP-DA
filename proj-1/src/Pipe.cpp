#include "Pipe.h"

Pipe::Pipe(const string &servicePointA, const string &servicePointB, int capacity, bool direction) : servicePointA(
        servicePointA), servicePointB(servicePointB), capacity(capacity), direction(direction) {}

const string &Pipe::getServicePointA() const {
    return servicePointA;
}

const string &Pipe::getServicePointB() const {
    return servicePointB;
}

int Pipe::getCapacity() const {
    return capacity;
}

bool Pipe::isDirection() const {
    return direction;
}