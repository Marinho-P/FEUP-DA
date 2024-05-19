#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include "vector"
#include <fstream>
#include <sstream>
#include "Edge.h"
#include "Vertex.h"
using namespace std;

/**
 * @brief Class representing a graph.
 */
class Graph {
    private:
        vector<Vertex> vertexSet; // Set of vertices in the graph
        vector<vector<Edge>> adj; // Adjacency list of the graph - Each vector is correlated to the index of the current node
        string current_edges_file; // Name of the file containing the edges
        string current_vertexes_file; // Name of the file containing the vertices
    public:
        /**
         * @brief Constructor for the Graph class.
         * @param edge_file Name of the file containing the edges.
         * @param vertex_file Name of the file containing the vertices (optional).
         */
        Graph(const string &edge_file, const string &vertex_file = "");

        /**
         * @brief Reads edges from a file.
         * @param edge_file Name of the file containing the edges.
         * @param comesWithNodes Boolean indicating if the file contains node information.
         * @return 0 on success, 1 on failure.
         * @details O(|E|) where |E| is the number of edges in the file.
         */
        int read_edges(const string &edge_file, bool comesWithNodes);

        /**
         * @brief Reads vertices from a file.
         * @param vertex_file Name of the file containing the vertices.
         * @return 0 on success, 1 on failure.
         * @details O(|V|) where |V| is the number of vertices in the file.
         */
        int read_vertexes(const string &vertex_file);

        /**
         * @brief Gets the name of the current edges file.
         * @return Constant reference to the string containing the name of the edges file.
         */
        const string &getCurrentEdgesFile() const;

        /**
         * @brief Gets the name of the current vertices file.
         * @return Constant reference to the string containing the name of the vertices file.
         */
        const string &getCurrentVertexesFile() const;

        /**
         * @brief Checks the edges of the graph.
         */
        void check_edges();

        /**
         * @brief Checks the vertices of the graph.
         */
        void check_vertexes();

        /**
         * @brief Executes the backtracking algorithm for the TSP.
         * @detailscomplexity O(N!) where N is the number of vertices.
         * @details This function applies the backtracking algorithm to find the minimum cost Hamiltonian path
         * in the graph. It starts from the initial vertex and explores all possible paths recursively,
         * keeping track of the visited vertices and current path cost. Once all paths are explored,
         * it selects the path with the minimum cost as the final solution.
         * @note If the number of vertices exceeds 20, the function prints a message indicating that
         * the execution is not reasonable due to computational complexity.
         */
        void backtracking();

        /**
         * @brief Recursively explores all possible paths using backtracking to find the minimum cost Hamiltonian path.
         * @param currentVertexId ID of the current vertex.
         * @param visited Array indicating visited vertices.
         * @param currentPath Current path taken.
         * @param finalPath Final path found.
         * @param nVertexes Number of vertices.
         * @param uniqueVertexes Number of unique vertices visited.
         * @param currentCost Current cost of the path.
         * @param finalCost Final cost of the path.
         * @details O(N!) where N is the number of vertices.
         * @details This function is called recursively to explore all possible paths starting from a given vertex.
         * It marks each visited vertex and updates the current path and cost accordingly. If a Hamiltonian path
         * with a lower cost than the current minimum is found, it updates the final path and cost accordingly.
         */
        void recursiveBacktracking(int currentVertexId,bool *visited,vector<int> &currentPath,vector<int> &finalPath,
                                   int nVertexes,int uniqueVertexes,double currentCost,double &finalCost);

        /**
         * @brief Executes the triangular approximation algorithm to find an approximate solution to the TSP.
         * @details O(N^2) where N is the number of vertices.
         * @details This function constructs a Minimum Spanning Tree (MST) using Prim's algorithm and performs a preorder walk on the MST
         * to generate a tour. The resulting tour is an approximate solution to the Traveling Salesman Problem (TSP).
         */
        void triangularApproximation();

        /**
         * @brief Gets the Minimum Spanning Tree (MST) using Prim's algorithm.
         * @param startingNode Starting node for Prim's algorithm (default is 0).
         * @return Vector of vectors of edges representing the MST.
         * @details O((V + E) log V) where V is the number of vertices and E is the number of edges.
         * @details This function constructs a Minimum Spanning Tree (MST) using Prim's algorithm starting from the specified node.
         * It returns the MST represented as a vector of vectors of Edges, where each inner vector contains the edges
         * incident to a vertex in the MST.
         */
        vector<vector<Edge>> getPrimMST(int startingNode = 0);

        /**
         * @brief Gets the distance between two vertices.
         * @param v id of the first vertex.
         * @param w id of the second vertex.
         * @return Distance between the vertices.
         */
        double getDistance(int v, int w);

        /**
         * @brief Preorder walk of the MST.
         * @param mst Vector of vectors of edges representing the MST.
         * @param currentVertex Current vertex in the walk.
         * @param visited Array indicating visited vertices.
         * @param tour Vector representing the tour.
         * @details O(N) where N is the number of vertices.
         */
        void preorderWalkMST(const vector<vector<Edge>> &mst, int currentVertex, bool *visited, vector<int> &tour);

        /**
         * @brief Gets the minimum cost of a given path.
         * @param path Vector representing the path.
         * @return Minimum cost of the path.
         * @details O(N) where N is the number of vertices in the path.
         */
        double getMinimumCost(vector<int> path);

        /**
         * @brief Executes the Christofides algorithm for the TSP.
         * @details O(N^3) where N is the number of vertices.
         * @details This function implements the Christofides algorithm for solving the TSP, which provides a solution
         * that is at most 1.5 times the optimal solution. It involves the following steps:
         * 1. Prompts the user to select a starting node.
         * 2. Constructs a Minimum Spanning Tree (MST) starting from the selected node.
         * 3. Finds a perfect matching for the MST.
         * 4. Combines the MST and the perfect matching to form an Eulerian tour.
         * 5. Converts the Eulerian tour into a Hamiltonian path by removing repeated nodes.
         * 6. Optimizes the Hamiltonian path using the 2-opt algorithm.
         * 7. Validates and calculates the cost of the final TSP path.
         * 8. Outputs the path, the minimum cost, and the execution time.
         */
        void Christofides();

        /**
         * @brief Finds a perfect matching in the MST.
         * @param mst Vector of vectors of edges representing the MST.
         * @param matchingVertexes Vector of pairs representing the matching vertices.
         * @details O(N^2) where N is the number of vertices.
         * @details This function identifies all vertices in the MST that have an odd degree and finds a perfect matching for them.
         * It does this by pairing each unvisited odd vertex with its closest unvisited neighbor.
         */
        void perfectMatching(vector<vector<Edge>> &mst, vector<pair<int, int>> &matchingVertexes);

        /**
        * @brief Finds an Euler tour in the graph.
        * @param matchingVertexes Vector of pairs representing the matching vertices.
        * @param path Vector representing the path.
        * @param mst Vector of vectors of edges representing the MST.
        * @param startingNode Starting node for the Euler tour.
        * @details O(V + E) where V is the number of vertices and E is the number of edges.
        */
        void euler_tour(vector<pair<int, int>> &matchingVertexes, vector<int> &path,vector<vector<Edge>> &mst,int startingNode);

        /**
        * @brief Makes a Hamiltonian path from a given path.
        * @param path Vector representing the path.
        * @return Vector representing the Hamiltonian path.
        * @details O(N) where N is the number of vertices in the path.
        */
        vector<int> make_hamiltonian(vector<int> &path);

        /**
         * @brief Creates a reduced distance matrix.
         * @return Vector of vectors of doubles representing the reduced distance matrix.
         * @details O(N^2) where N is the number of vertices.
         */
        vector<vector<double>> createReducedMatrix();

        /**
         * @brief Optimizes a tour using the 2-opt algorithm.
         * @param tour Vector representing the tour.
         * @param distanceMatrix Vector of vectors of doubles representing the distance matrix.
         * @complexity O(N^2) where N is the number of vertices.
         * @details This function applies the 2-opt optimization technique to improve the given tour by iteratively
         * reversing segments of the tour that reduce the total distance. The process continues until no
         * further improvements can be made.
         */
        void twoOptSearchOptimization(vector<int> &tour, const vector<vector<double>> &distanceMatrix);

        /**
         * @brief Depth-First Search (DFS) for the Euler tour.
         * @param vertex Current vertex in the DFS.
         * @param visited Vector indicating visited vertices.
         * @param mst Vector of vectors of edges representing the MST.
         * @param path Vector representing the path.
         * @details O(V + E) where V is the number of vertices and E is the number of edges.
         */
        void dfs(int vertex, vector<bool> &visited, vector<vector<Edge>> &mst, vector<int> &path);

        /**
         * @brief Checks if a given path is circular.
         * @param path Vector representing the path.
         * @param startingNode Starting node for the path (default is 0).
         * @return True if the path is circular, false otherwise.
         * @details O(N) where N is the number of vertices in the path.
         */
        bool isCircularPath(vector<int> &path, int startingNode = 0);

        /**
         * @brief Checks if a given path is feasible.
         * @param startingNode The node from which to start the BFS.
         * @param distanceMatrix A matrix representing the distances between vertices.
         * @return True if the path is feasible, false otherwise.
         * @details O(N^2) where N is the number of vertices.
         * @detailsThis function performs a Breadth-First Search (BFS) to determine if all vertices in the graph are reachable
         * from the specified starting node, thus checking if a feasible path exists that visits all vertices.
         */
        bool isFeasiblePath(int startingNode, const vector<vector<double>> &distanceMatrix);

        /**
         * @brief Calculates the cost of a path without using coordinates.
         * @param path Vector representing the path.
         * @param minimumCost Reference to the variable storing the minimum cost.
         * @return True on success, false otherwise.
         * @details O(N) where N is the number of vertices in the path.
         */
        bool calculatePathCostWithoutCoordinates(vector<int> path, double &minimumCost);

        /**
         * @brief Checks the distance between two vertices.
         * @param v ID of the first vertex.
         * @param w ID of the second vertex.
         * @return Distance between the vertices.
         * @details O(N) where N is the number of vertices.
         */
        double checkDistance(int v, int w);

        /**
         * @brief Checks the current files being used.
         */
        void checkCurrentFiles();

        /**
         * @brief Runs all algorithms.
         */
        void runAllAlgorithms();

        /**
         * @brief Executes an optimized triangular approximation algorithm for the Traveling Salesman Problem (TSP).
         * @details This function creates a distance matrix, clusters vertices based on average edge distance, and uses Prim's algorithm
         * to construct a Minimum Spanning Tree (MST) for each cluster. It then performs a pre-order traversal of the MST to
         * generate an approximate solution for the TSP. The path and the associated cost are outputted.
         * @details The complexity is approximately O(V^2) due to the distance matrix creation and MST construction.
         */
        void optimizedTriangularApproximation();

        /**
         * @brief Creates clusters of vertices based on a distance threshold.
         * @param dist Distance threshold for clustering.
         * @param newAdj Vector of vectors of vertices representing the new adjacency list of the new clusters.
         * @param anchors Vector of vertices that will act as cluster anchors.
         * @details O(V^2) where V is the number of vertices.
         * @details This function iterates through all vertices, assigning each vertex to an anchor
         * if the distance is below the threshold. If a vertex cannot be assigned to any
         * existing anchor, it becomes a new anchor. Each cluster is stored in the `clusters`
         * vector, and the adjacency list for the new clusters is updated accordingly.
         */
        void createClusters(double dist,  vector<vector<Vertex>> &newAdj, vector<Vertex> &anchors);

        /**
         * @brief Generates the Minimum Spanning Tree (MST) using an optimized Prim's algorithm.
         * @param visited Array of booleans indicating whether each vertex has been visited.
         * @param start The starting vertex for Prim's algorithm.
         * @param clusterAdj Vector of vectors of vertices representing the adjacency list of the clusters.
         * @param mstAdj Vector of vectors of vertices representing the MST adjacency list.
         * @details O(E log V) where E is the number of edges and V is the number of vertices.
         * @details This function uses a priority queue to efficiently find the next vertex to add to the MST.
         * Each vertex is connected to its parent with the minimum edge weight, and the adjacency list
         * for the MST is updated accordingly. The `visited` array ensures that each vertex is only
         * processed once.
         */
        void getPrimMSTopt(bool * visited, Vertex &start, vector<vector<Vertex>>& clusterAdj,
                                   vector<vector<Vertex>> &mstAdj);


        /**
        * @brief Performs a pre-order traversal of the Minimum Spanning Tree (MST).
        * @param path Vector of integers where each integer represents the ID of a vertex in the traversal path.
        * @param visited Array of booleans indicating whether each vertex has been visited.
        * @param start The starting vertex for the pre-order traversal.
        * @param mstAdj Vector of vectors of vertices representing the adjacency list of the MST.
        * @details O(N) where N is the number of vertices.
        * @details This function recursively visits each vertex in a pre-order manner, starting from the given start vertex.
        * It marks each visited vertex and adds it to the traversal path. The traversal continues to all adjacent
        * vertices that have not been visited yet.
        */
        void preorderWalkOpt(vector<int>& path,bool* visited, Vertex& start, vector<vector<Vertex>>& mstAdj);

};

#endif