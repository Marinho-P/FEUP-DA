#ifndef PROJETODA2_GRAPH_H
#define PROJETODA2_GRAPH_H

#include <string>
#include "vector"
#include <fstream>
#include <sstream>
#include "Edge.h"
#include "Vertex.h"
using namespace std;
class Graph {
    private:
        vector<Vertex> vertexSet;
        vector<vector<Edge>> adj; /* Each vector is correlated to the index of the current node */
        string current_edges_file;
        string current_vertexes_file;
    public:
        Graph(const string &edge_file, const string &vertex_file = "");
        int read_edges(const string &edge_file, bool comesWithNodes);
        int read_vertexes(const string &vertex_file);
        const vector<Vertex> &getVertexSet() const;
        const vector<vector<Edge>> &getAdj() const;
        const string &getCurrentEdgesFile() const;
        const string &getCurrentVertexesFile() const;

    void check_edges();
};




#endif //PROJETODA2_GRAPH_H
