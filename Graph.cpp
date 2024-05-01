#include <iostream>
#include "Graph.h"


Graph::Graph(const string &edges, const string &vertexes){

    if(vertexes.empty()){
        if(read_edges(edges,false)) exit(-1);
    }else{
        if(read_edges(edges,true)) exit(-1);
        if(read_vertexes(vertexes)) exit(-1);
    }


    current_edges_file = edges;
    current_vertexes_file = vertexes;

}


const vector<Vertex> &Graph::getVertexSet() const {
    return vertexSet;
}

const vector<vector<Edge>> &Graph::getAdj() const {
    return adj;
}

const string &Graph::getCurrentEdgesFile() const {
    return current_edges_file;
}

const string &Graph::getCurrentVertexesFile() const {
    return current_vertexes_file;
}

int Graph::read_edges(const string &edge_file,bool comesWithNodes){
    ifstream file(edge_file);
    int maxId = INT_MIN;
    int orig_id, dest_id;
    double dist;
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        if(line[0] == '0'){ // If it has no header
            string originVertex, destinyVertex , distance;
            istringstream iss(line);
            getline(iss,originVertex,',');
            getline(iss,destinyVertex,',');
            getline(iss,distance,',');
            orig_id = stoi(originVertex);
            dest_id = stoi(destinyVertex);
            dist = stod(distance);
            Edge start = Edge(orig_id,dist);
            Edge finish = Edge(dest_id,dist);
            maxId = max(orig_id,dest_id);
            while(adj.size() <= maxId){
                adj.emplace_back();
            }
            adj[orig_id].push_back(finish);
            adj[dest_id].push_back(start);
        }
        while (getline(file,line)){
            string originVertex, destinyVertex , distance;
            istringstream iss(line);
            getline(iss,originVertex,',');
            getline(iss,destinyVertex,',');
            getline(iss,distance,',');
            orig_id = stoi(originVertex);
            dest_id = stoi(destinyVertex);
            dist = stod(distance);
            Edge start = Edge(orig_id,dist);
            Edge finish = Edge(dest_id,dist);
            maxId = max(orig_id,dest_id);
            while(adj.size() <= maxId){
                adj.emplace_back();
            }
            adj[orig_id].push_back(finish);
            adj[dest_id].push_back(start);
        }

    }else {
        return 1;
    }
    if(!comesWithNodes){
        for(int id = 0; id <= maxId;id++){
            vertexSet.push_back(Vertex(0,0,id));
        }
    }
    return 0;
}


int Graph::read_vertexes(const string &node_file){
    ifstream file(node_file);
    int id;
    double latitude , longitude;
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string lat,log, id_str;
            istringstream iss(line);
            getline(iss,id_str,',');
            getline(iss,log,',');
            getline(iss,lat,',');
            id = stoi(id_str);
            latitude = stod(lat);
            longitude = stod(log);
            vertexSet.push_back(Vertex(latitude,longitude,id));
        }
    }else{
        return 1;
    }
    return 0;
}

void Graph::check_edges(){
    cout << "Edges file being used: " << current_edges_file << endl;
    for(int i = 0; i < adj.size() ; i++){
        cout << "Adj to Vertex[" << i << "]:" << endl;
        for(auto edge:adj[i]){
            cout << edge.getDestiny() << "----" << "dist = " << edge.getDistance() << endl;
        }
    }
}