#include "Station.h"

Station::Station(int id, string code):NodeData(id, code, nodeType::STATION){}

bool Station::operator==(const Station& other) const{
    return other.id == id;
}