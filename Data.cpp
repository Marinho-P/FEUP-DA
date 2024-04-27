#include <fstream>
#include <sstream>
#include <cmath>
#include "Data.h"
#include "data_structures/Graph.h"


Data::Data() {
    graph = new Graph<int>();
}


void Data::readToyGraph(string inputCSV){
    int sourceNode, destNode;
    double distance;
    std::ifstream file(inputCSV);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            istringstream iss(line);
            string sourceNode_str,destNode_str,distance_str;
            getline(iss,sourceNode_str,',');
            getline(iss,destNode_str,',');
            getline(iss,distance_str,',');
            sourceNode = stoi(sourceNode_str);
            destNode = stoi(destNode_str);
            distance = stod(distance_str);
            if(graph->findVertex(sourceNode) == nullptr){
                graph->addVertex(sourceNode);
            }
            if(graph->findVertex(destNode) == nullptr){
                graph->addVertex(destNode);
            }
            graph->addBidirectionalEdge(sourceNode,destNode,distance);
        }
    }else {
        cout << "Could not open the file\n";
    }
}

void Data::verifyGraph(){
    for(auto vertex:graph->getVertexSet()){
        for(auto adj:vertex->getAdj()){
            cout << vertex->getInfo() << " ---> "<< adj->getDest()->getInfo() << endl;
        }
    }
}