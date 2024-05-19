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
        void check_vertexes();
        void backtracking();
        void recursiveBacktracking(int currentVertexId,bool *visited,vector<int> &currentPath,vector<int> &finalPath,int nVertexes,int uniqueVertexes,double currentCost,double &finalCost);
        void triangularApproximation();
        vector<vector<Edge>> getPrimMST(int startingNode = 0);
        double getDistance(int v, int w);

    void preorderWalkMST(const vector<vector<Edge>> &mst, int currentVertex, bool *visited, vector<int> &tour);

    double getMinimumCost(vector<int> path);

    void Christofides();

    void perfectMatching(vector<vector<Edge>> &mst, vector<pair<int, int>> &matchingVertexes);

    void euler_tour(vector<pair<int, int>> &matchingVertexes, vector<int> &path,vector<vector<Edge>> &mst,int startingNode);

    vector<int> make_hamiltonian(vector<int> &path);

    vector<vector<double>> createReducedMatrix();

    void twoOptSearchOptimization(vector<int> &tour, const vector<vector<double>> &distanceMatrix);

    void dfs(int vertex, vector<bool> &visited, vector<vector<Edge>> &mst, vector<int> &path);

    bool isCircularPath(vector<int> &path, int startingNode = 0);

    bool isFeasiblePath(int startingNode, const vector<vector<double>> &distanceMatrix);

    bool calculatePathCostWithoutCoordinates(vector<int> path, double &minimumCost);

    double checkDistance(int v, int w);

    void checkCurrentFiles();

    void runAllAlgorithms();

    void optimizedTriangularApproximation();



    void createClusters(double dist, vector<vector<double>> distanceMatrix, vector<vector<Vertex>> &newAdj,
                        vector<Vertex> &anchors);






    static void
    getPrimMSTopt(bool * visited, Vertex &start, vector<vector<Vertex>>& clusterAdj, vector<vector<double>>& distanceMatrix,
                  vector<vector<Vertex>> &mstAdj);
    void preorderWalkOpt(vector<int>& path,bool* visited, Vertex& start, vector<vector<Vertex>>& mstAdj);
};




#endif //PROJETODA2_GRAPH_H
