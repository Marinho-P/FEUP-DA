#include <iostream>
#include <chrono>
using namespace std::chrono;
#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "algorithm"


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

void Graph::check_vertexes(){
    if(current_vertexes_file.empty()){
        cout << "Vertex file isn't being used" << endl;
    }else{
        cout << "Vertexes file being used: " << current_vertexes_file << endl;
        for(auto vertex: vertexSet){
            cout << "Vertex["<<vertex.getId() <<  "] with coordinates(" << vertex.getLatitude() << "," << vertex.getLongitude() << ")\n";
        }
    }
}

double startingNodeIsReachable(vector<Edge> vector){
    for(auto edge:vector){
        if(edge.getDestiny() == 0){
            return edge.getDistance();
        }
    }
    return 0.0;
}


void Graph::recursiveBacktracking(int currentVertexId,bool *visited,vector<int> &currentPath,vector<int> &finalPath,int nVertexes,int uniqueVertexes,double currentCost,double &finalCost){
    if(currentCost > finalCost) return;

    double distanceToReturn = startingNodeIsReachable(adj[currentVertexId]);

    if(uniqueVertexes == nVertexes && distanceToReturn){
         if(currentCost + distanceToReturn < finalCost){
             finalCost = currentCost + distanceToReturn;
             finalPath = currentPath;
         }
        return;
    }

    for (Edge edge : adj[currentVertexId]) {
        if(!visited[edge.getDestiny()]){
            visited[edge.getDestiny()] = true;
            currentPath.push_back(edge.getDestiny());
            recursiveBacktracking(edge.getDestiny(),visited,currentPath,finalPath,nVertexes,uniqueVertexes + 1,currentCost + edge.getDistance(),finalCost);
            currentPath.pop_back();
            visited[edge.getDestiny()] = false;
        }
    }
}

double Graph::getMinimumCost(vector<int> path){
    double minimumCost = 0;
    for(int i = 0; i < path.size() - 1;i++){
        minimumCost+= getDistance(path[i],path[i+1]);
    }
    return minimumCost;
}

double Graph::getDistance(int v, int w){
    for(auto edge:adj[v]){
        if(edge.getDestiny() == w){
            return edge.getDistance();
        }
    }
    return vertexSet[v].calculateDistanceToVertex(vertexSet[w]);
}

void Graph::backtracking(){
    int nVertexes = vertexSet.size();
    bool* visited = new bool[nVertexes];
    for (int i = 0; i < nVertexes; ++i) {
        visited[i] = false;
    }
    visited[0] = true; // set true starting vertex

    vector<int> currentPath = {0}; // current path being taken
    vector<int> finalPath;
    double currentCost = 0;
    double finalCost =  INFINITY;

    auto startingPoint = high_resolution_clock::now();
    recursiveBacktracking(0,visited,currentPath,finalPath,nVertexes,1,currentCost,finalCost);
    auto finishPoint =  high_resolution_clock::now();
    auto duration_ = duration_cast<duration<double>>(finishPoint - startingPoint);

    delete[] visited;
    cout << "-- Backtracking complete --" << endl;
    cout << "Elapsed time: " << duration_.count() << " seconds" << endl;
    cout << "Minimum cost to travel: " << finalCost << endl;
    cout << "Path taken: ";
    int j = 0;
    finalPath.push_back(0);
    for(auto vertex:finalPath){
        if( j == finalPath.size()-1){
            cout << vertex << endl;

        }else{
            cout << vertex << "-->";
        }
        j++;
    }
}


vector<vector<Edge>> Graph::getPrimMST(){
    MutablePriorityQueue<Vertex> q;
    vector<int> path = {0};
    vector<pair<int, float>>  parent (vertexSet.size(), {-1, 0.0});
    bool* visited = new bool[vertexSet.size()];

    for (int i = 0; i < vertexSet.size(); ++i) {
        visited[i] = false;
        vertexSet[i].setDist(INFINITY);
    }
    visited[0] = true;

    for (auto& vertex : vertexSet) { // Use reference to avoid copying
        q.insert(&vertex); // Insert pointer to the original element
    }

    vertexSet[0].setDist(0);
    q.decreaseKey(&vertexSet[0]);

    while (!q.empty()){
        Vertex* vertex = q.extractMin();
        visited[vertex->getId()] = true;

        for(Edge &u:adj[vertex->getId()]) {
            Vertex &minVertex = vertexSet[u.getDestiny()];
            if (!visited[minVertex.getId()] && u.getDistance() < minVertex.getDist()) {
                minVertex.setDist(u.getDistance());
                parent[minVertex.getId()] = {vertex->getId(),minVertex.getDist()};
                q.decreaseKey(&minVertex);
            }
        }
    }
    vector<vector<Edge>> mst (vertexSet.size());

    for(int i = 0; i < vertexSet.size(); i++){
        if(parent[i].first != -1){
            int w = parent[i].first;
            double distance = parent[i].second;
            mst[w].push_back({i, distance});
            mst[i].push_back({w, distance}); // mst bidirectional
        }
    }
    delete [] visited;
    return mst;
}

void Graph::preorderWalkMST(const vector<vector<Edge>> &mst, int currentVertex,bool *visited, vector<int> &tour){
    visited[currentVertex] = true;
    tour.push_back(currentVertex);

    vector<Edge> vector = mst[currentVertex];
    sort(vector.begin(), vector.end(), [&](const Edge &edge, const Edge &other) {
        return vertexSet[edge.getDestiny()].calculateDistanceToVertex(vertexSet[currentVertex]) < vertexSet[other.getDestiny()].calculateDistanceToVertex(vertexSet[currentVertex]);
    });

    for (const Edge &edge : vector) {
        if (!visited[edge.getDestiny()]) {
            preorderWalkMST(mst, edge.getDestiny(), visited, tour);
        }
    }
}

void Graph::triangularApproximation(){

    bool* v = new bool[vertexSet.size()];
    for (int i = 0; i < vertexSet.size(); ++i) {
        v[i] = false;
    }

    auto startingPoint = high_resolution_clock::now();
    vector<int> tour;
    vector<vector<Edge>> mst = getPrimMST();
    preorderWalkMST(mst,0,v,tour);
    auto finishPoint =  high_resolution_clock::now();

    auto duration_ = duration_cast<duration<double>>(finishPoint - startingPoint);
    delete [] v;
    tour.push_back(0);

    cout << "-- Triangular Approximation complete --" << endl;
    cout << "Elapsed time: " << duration_.count() << " seconds" << endl;
    cout << "Minimum cost to travel: "  << getMinimumCost(tour)<< endl;
    cout << "Path taken: ";
    int j = 0;
    for(auto vertex:tour){
        if( j == tour.size()-1){
            cout << vertex << endl;

        }else{
            cout << vertex << "-->";
        }
        j++;
    }
}


