#include "NodeData.h"

NodeData::NodeData(int id, const string &code, nodeType type){
    this->id = id;
    this->type = type;
    this->code = code;
}

nodeType NodeData::getType() const {
    return type;
}

int NodeData::getId() const {
    return id;
}

const string &NodeData::getCode() const {
    return code;
}

bool NodeData::operator==(const NodeData& other) const{
    return other.getType() == type && other.code == code;
}