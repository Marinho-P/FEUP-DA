#include "City.h"

const string &City::getName() const {
    return name;
}

double City::getDemand() const {
    return demand;
}

int City::getPopulation() const {
    return population;
}

City::City(const string &name, const int &id, const string &delivery_code, const double &demand, const int &population): NodeData(id, delivery_code, nodeType::CITY){
    this->name = name;
    this->demand = demand;
    this->population = population;
}

bool City::operator==(const City& other) const{
    return id == other.id;
}