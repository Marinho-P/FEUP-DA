#include "Reservoir.h"

Reservoir::Reservoir(const std::string &name, const std::string &municipality, const int &id, const std::string &code,const int &maxDelivery): NodeData(id,code,RESERVOIR) {
    this->name = name;
    this->municipality = municipality;
    this->maxDelivery = maxDelivery;
}

const string &Reservoir::getName() const {
    return name;
}

const string &Reservoir::getMunicipality() const {
    return municipality;
}

int Reservoir::getMaxDelivery() const {
    return maxDelivery;
}

bool Reservoir::operator==(const Reservoir& other) const{
    return other.id == id;
}