#include <iostream>
#include <chrono>
#include <queue>
#include <stack>
#include <unordered_set>
#include <climits>

using namespace std::chrono;

#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "algorithm"


Graph::Graph(const string &edges, const string &vertexes) {
    if (vertexes.empty()) {
        if (read_edges(edges, false)) exit(-1);
    } else {
        if (read_edges(edges, true)) exit(-1);
        if (read_vertexes(vertexes)) exit(-1);
    }
    current_edges_file = edges;
    current_vertexes_file = vertexes;
}



const string &Graph::getCurrentEdgesFile() const {
    return current_edges_file;
}



const string &Graph::getCurrentVertexesFile() const {
    return current_vertexes_file;
}



int Graph::read_edges(const string &edge_file, bool comesWithNodes) {
    ifstream file(edge_file);
    int maxId = INT_MIN;
    int orig_id, dest_id;
    double dist;
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip header line
        if (line[0] == '0') { // If it has no header
            string originVertex, destinyVertex, distance;
            istringstream iss(line);
            getline(iss, originVertex, ',');
            getline(iss, destinyVertex, ',');
            getline(iss, distance, ',');
            orig_id = stoi(originVertex);
            dest_id = stoi(destinyVertex);
            dist = stod(distance);
            Edge start = Edge(orig_id, dist);
            Edge finish = Edge(dest_id, dist);
            maxId = max(orig_id, dest_id);
            while (adj.size() <= maxId) {
                adj.emplace_back();
            }
            adj[orig_id].push_back(finish);
            adj[dest_id].push_back(start);
        }
        while (getline(file, line)) {
            string originVertex, destinyVertex, distance;
            istringstream iss(line);
            getline(iss, originVertex, ',');
            getline(iss, destinyVertex, ',');
            getline(iss, distance, ',');
            orig_id = stoi(originVertex);
            dest_id = stoi(destinyVertex);
            dist = stod(distance);
            Edge start = Edge(orig_id, dist);
            Edge finish = Edge(dest_id, dist);
            maxId = max(orig_id, dest_id);
            while (adj.size() <= maxId) {
                adj.emplace_back();
            }
            adj[orig_id].push_back(finish);
            adj[dest_id].push_back(start);
        }

    } else {
        return 1;
    }
    if (!comesWithNodes) {
        for (int id = 0; id <= maxId; id++) {
            vertexSet.push_back(Vertex(0, 0, id));
        }
    }
    return 0;
}



int Graph::read_vertexes(const string &node_file) {
    ifstream file(node_file);
    int id;
    double latitude, longitude;
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip header line
        while (getline(file, line)) {
            string lat, log, id_str;
            istringstream iss(line);
            getline(iss, id_str, ',');
            getline(iss, log, ',');
            getline(iss, lat, ',');
            id = stoi(id_str);
            latitude = stod(lat);
            longitude = stod(log);
            vertexSet.push_back(Vertex(latitude, longitude, id));
        }
    } else {
        return 1;
    }
    return 0;
}



void Graph::check_edges() {
    cout << "Edges file being used: " << current_edges_file << endl;
    for (int i = 0; i < adj.size(); i++) {
        cout << "Adj to Vertex[" << i << "]:" << endl;
        for (auto edge: adj[i]) {
            cout << edge.getDestiny() << "----" << "dist = " << edge.getDistance() << endl;
        }
    }
}



void Graph::check_vertexes() {
    if (current_vertexes_file.empty()) {
        cout << "Vertex file isn't being used" << endl;
    } else {
        cout << "Vertexes file being used: " << current_vertexes_file << endl;
        for (auto vertex: vertexSet) {
            cout << "Vertex[" << vertex.getId() << "] with coordinates(" << vertex.getLatitude() << ","
                 << vertex.getLongitude() << ")\n";
        }
    }
}



double startingNodeIsReachable(vector<Edge> vector) {
    for (auto edge: vector) {
        if (edge.getDestiny() == 0) {
            return edge.getDistance();
        }
    }
    return 0.0;
}



void Graph::recursiveBacktracking(int currentVertexId, bool *visited, vector<int> &currentPath, vector<int> &finalPath,
                                  int nVertexes, int uniqueVertexes, double currentCost, double &finalCost) {
    if (currentCost > finalCost) return;

    double distanceToReturn = startingNodeIsReachable(adj[currentVertexId]);

    if (uniqueVertexes == nVertexes && distanceToReturn) {
        if (currentCost + distanceToReturn < finalCost) {
            finalCost = currentCost + distanceToReturn;
            finalPath = currentPath;
        }
        return;
    }

    for (Edge edge: adj[currentVertexId]) {
        if (!visited[edge.getDestiny()]) {
            visited[edge.getDestiny()] = true;
            currentPath.push_back(edge.getDestiny());
            recursiveBacktracking(edge.getDestiny(), visited, currentPath, finalPath, nVertexes, uniqueVertexes + 1,
                                  currentCost + edge.getDistance(), finalCost);
            currentPath.pop_back();
            visited[edge.getDestiny()] = false;
        }
    }
}



double Graph::getMinimumCost(vector<int> path) {
    double minimumCost = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        minimumCost += getDistance(path[i], path[i + 1]);
    }
    return minimumCost;
}



double Graph::getDistance(int v, int w) {
    for (auto edge: adj[v]) {
        if (edge.getDestiny() == w) {
            return edge.getDistance();
        }
    }
    return vertexSet[v].calculateDistanceToVertex(vertexSet[w]);
}



void Graph::backtracking() {
    int nVertexes = vertexSet.size();
    if (nVertexes > 20) {
        cout << "Unreasonable amount of nodes will not execute" << endl;
        return;
    }
    bool *visited = new bool[nVertexes];
    for (int i = 0; i < nVertexes; ++i) {
        visited[i] = false;
    }
    visited[0] = true; // set true starting vertex

    vector<int> currentPath = {0}; // current path being taken
    vector<int> finalPath;
    double currentCost = 0;
    double finalCost = INFINITY;

    auto startingPoint = high_resolution_clock::now();
    recursiveBacktracking(0, visited, currentPath, finalPath, nVertexes, 1, currentCost, finalCost);
    auto finishPoint = high_resolution_clock::now();
    auto duration_ = duration_cast<duration<double>>(finishPoint - startingPoint);

    delete[] visited;
    cout << "-- Backtracking complete --" << endl;
    cout << "Elapsed time: " << duration_.count() << " seconds" << endl;
    cout << "Minimum cost to travel: " << finalCost << endl;
    cout << "Path taken: ";
    int j = 0;
    finalPath.push_back(0);
    for (auto vertex: finalPath) {
        if (j == finalPath.size() - 1) {
            cout << vertex << endl;
        } else {
            cout << vertex << "-->";
        }
        j++;
    }
}


vector<vector<Edge>> Graph::getPrimMST(int startingNode) {
    MutablePriorityQueue<Vertex> q;
    vector<pair<int, float>> parent(vertexSet.size(), {-1, 0.0});
    bool *visited = new bool[vertexSet.size()];

    for (int i = 0; i < vertexSet.size(); ++i) {
        visited[i] = false;
        vertexSet[i].setDist(INFINITY);
    }
    visited[startingNode] = true;

    for (auto &vertex: vertexSet) { // Use reference to avoid copying
        q.insert(&vertex); // Insert pointer to the original element
    }

    vertexSet[startingNode].setDist(0);
    q.decreaseKey(&vertexSet[startingNode]);

    while (!q.empty()) {
        Vertex *vertex = q.extractMin();
        visited[vertex->getId()] = true;

        for (Edge &u: adj[vertex->getId()]) {
            Vertex &minVertex = vertexSet[u.getDestiny()];
            if (!visited[minVertex.getId()] && u.getDistance() < minVertex.getDist()) {
                minVertex.setDist(u.getDistance());
                parent[minVertex.getId()] = {vertex->getId(), minVertex.getDist()};
                q.decreaseKey(&minVertex);
            }
        }
    }
    vector<vector<Edge>> mst(vertexSet.size());

    for (int i = 0; i < vertexSet.size(); i++) {
        if (parent[i].first != -1) {
            int w = parent[i].first;
            double distance = parent[i].second;
            mst[w].push_back({i, distance});
            mst[i].push_back({w, distance}); // mst bidirectional
        }
    }
    delete[] visited;
    return mst;
}



void Graph::preorderWalkMST(const vector<vector<Edge>> &mst, int currentVertex, bool *visited, vector<int> &tour) {
    visited[currentVertex] = true;
    tour.push_back(currentVertex);

    vector<Edge> vector = mst[currentVertex];

    for (const Edge &edge: vector) {
        if (!visited[edge.getDestiny()]) {
            preorderWalkMST(mst, edge.getDestiny(), visited, tour);
        }
    }
}



void Graph::triangularApproximation() {

    bool *v = new bool[vertexSet.size()];
    for (int i = 0; i < vertexSet.size(); ++i) {
        v[i] = false;
    }

    auto startingPoint = high_resolution_clock::now();
    vector<int> tour;
    vector<vector<Edge>> mst = getPrimMST();
    preorderWalkMST(mst, 0, v, tour);
    auto finishPoint = high_resolution_clock::now();

    auto duration_ = duration_cast<duration<double>>(finishPoint - startingPoint);
    delete[] v;
    tour.push_back(0);

    cout << "-- Triangular Approximation complete --" << endl;
    cout << "Elapsed time: " << duration_.count() << " seconds" << endl;
    cout << "Minimum cost to travel: " << getMinimumCost(tour) << endl;
    cout << "Path taken: ";
    int j = 0;
    for (auto vertex: tour) {
        if (j == tour.size() - 1) {
            cout << vertex << endl;
        } else {
            cout << vertex << "-->";
        }
        j++;
    }
}



void Graph::perfectMatching(vector<vector<Edge>> &mst, vector<pair<int, int>> &matchingVertexes) {
    bool *visited = new bool[mst.size()];
    for (int i = 0; i < mst.size(); ++i) {
        visited[i] = false;
    }

    // Find nodes with odd degrees in T to get subgraph O
    vector<int> oddVertices;
    for (int vertexId = 0; vertexId < mst.size(); vertexId++) {
        if (mst[vertexId].size() % 2 != 0) {
            oddVertices.push_back(vertexId);
        }
    }

    for (int vertex: oddVertices) {
        if (!visited[vertex]) {
            double minDistance = INFINITY;
            int matchingVertex = -1;

            for (const Edge &e: mst[vertex]) {
                int v = e.getDestiny();
                double cost = e.getDistance();
                if (!visited[v] && cost < minDistance) {
                    minDistance = cost;
                    matchingVertex = v;
                }
            }

            if (matchingVertex != -1) {
                matchingVertexes.push_back({vertex, matchingVertex});
                visited[vertex] = true;
                visited[matchingVertex] = true;
            }
        }
    }
    delete[] visited;
}



//find an euler circuit
void Graph::euler_tour(vector<pair<int, int>> &matchingVertexes, vector<int> &path, vector<vector<Edge>> &mst,
                       int startingNode) {
    // Para criar o path temos de fazer a mst e dps juntar os matching vertexes
    vector<bool> visited(vertexSet.size(), false); // Initialize visited array
    dfs(startingNode, visited, mst, path);

    for (auto pairVertices: matchingVertexes) {
        path.push_back(pairVertices.first);
        path.push_back(pairVertices.second);
    }
}



vector<int> Graph::make_hamiltonian(vector<int> &path) {
    //remove visited nodes from Euler tour
    unordered_set<int> uniqueVertices;
    vector<int> newPath;
    for (auto vertex: path) {
        if (uniqueVertices.insert(vertex).second) {
            newPath.push_back(vertex);
        }
    }
    return newPath;
}



vector<vector<double>> Graph::createReducedMatrix() {
    vector<vector<double>> distanceMatrix(vertexSet.size(), vector<double>(vertexSet.size(), INFINITY));
    for (auto vertex: vertexSet) {
        for (auto edge: adj[vertex.getId()]) {
            distanceMatrix[vertex.getId()][edge.getDestiny()] = edge.getDistance();
        }
    }
    return distanceMatrix;
}



void Graph::twoOptSearchOptimization(vector<int> &tour, const vector<vector<double>> &distanceMatrix) {
    bool improvement = true;

    while (improvement) {
        improvement = false;

        for (int i = 1; i < tour.size() - 1; ++i) {
            for (int k = i + 1; k < tour.size(); ++k) {
                // Get the indices of the nodes forming the current edge pair
                int edge1_start = tour[i - 1];
                int edge1_end = tour[i];
                int edge2_start = tour[k];
                int edge2_end = (k == tour.size() - 1) ? tour[0] : tour[k + 1];

                // Calculate the distances of the two edges and the reversed edges
                double original_length =
                        distanceMatrix[edge1_start][edge1_end] + distanceMatrix[edge2_start][edge2_end];
                double reversed_length =
                        distanceMatrix[edge1_start][edge2_start] + distanceMatrix[edge1_end][edge2_end];

                // If reversing the order results in a shorter tour, accept the modification
                if (reversed_length < original_length) {
                    reverse(tour.begin() + i, tour.begin() + k + 1);
                    improvement = true;
                }
            }
        }
    }
}



void Graph::dfs(int vertex, vector<bool> &visited, vector<vector<Edge>> &mst, vector<int> &path) {
    visited[vertex] = true; // Mark the current vertex as visited
    for (const Edge &edge: mst[vertex]) {
        int dest = edge.getDestiny();
        if (!visited[dest]) { // If the adjacent vertex hasn't been visited
            path.push_back(vertex); // Add the current vertex to the path
            path.push_back(dest); // Add the adjacent vertex to the path
            dfs(dest, visited, mst, path); // Recursively visit the adjacent vertex
        }
    }
}



bool Graph::isFeasiblePath(int startingNode, const vector<vector<double>> &distanceMatrix) {
    vector<bool> visited(vertexSet.size(), false); // Initialize visited array
    queue<int> q; // Queue for BFS

    // Start BFS from the starting node
    q.push(startingNode);
    visited[startingNode] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Check all adjacent nodes of the current node
        for (int neighbor = 0; neighbor < vertexSet.size(); ++neighbor) {
            // If there's a connection to the current node and the neighbor hasn't been visited yet
            if (distanceMatrix[current][neighbor] != INFINITY && !visited[neighbor]) {
                // Mark the neighbor as visited and add it to the queue
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    // Check if all nodes were visited, indicating a path back to the starting node
    return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}



void Graph::Christofides() {
    int startingNode;
    cout << "Choose a node between 0 and " << vertexSet.size() - 1 << " to start from:" << endl;
    cin >> startingNode;

    if (cin.fail() || cin.peek() != '\n' || startingNode > vertexSet.size() - 1 || startingNode < 0) {
        cerr << "Invalid input" << endl;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        Christofides();
    }

    vector<int> path;
    vector<pair<int, int>> matchingVertexes;

    auto startingPoint = high_resolution_clock::now();

    vector<vector<Edge>> mst = getPrimMST(startingNode);
    // encontrar perfect matching
    perfectMatching(mst, matchingVertexes);
    // juntar as edges da mst com o perfect matching
    euler_tour(matchingVertexes, path, mst, startingNode);
    // Retirar nodes repetidos.
    vector<int> finalPath = make_hamiltonian(path);

    vector<vector<double>> distMatrix = createReducedMatrix();

    if (!isFeasiblePath(startingNode, distMatrix)) {
        cout << "There is no available path" << endl;
        return;
    }

    //reverse the order of nodes between pairs of their edges
    twoOptSearchOptimization(finalPath, distMatrix);

    auto finishPoint = high_resolution_clock::now();

    auto duration_ = duration_cast<duration<double>>(finishPoint - startingPoint);

    finalPath.push_back(startingNode);

    if (isCircularPath(finalPath, startingNode)) {
        cout << "Path made is incorrect" << endl;
        return;
    }
    double finalCost = 0;

    if (!calculatePathCostWithoutCoordinates(finalPath, finalCost)) {
        cout << "There is no available path" << endl;
        return;
    }
    cout << "-- Christofides Algorithm complete --" << endl;
    cout << "Elapsed time: " << duration_.count() << " seconds" << endl;
    cout << "Minimum cost to travel: " << finalCost << endl;
    cout << "Path taken: ";
    int j = 0;
    for (auto vertex: finalPath) {
        if (j == finalPath.size() - 1) {
            cout << vertex << endl;
        } else {
            cout << vertex << "-->";
        }
        j++;
    }
}



double Graph::checkDistance(int v, int w) {
    for (auto edge: adj[v]) {
        if (edge.getDestiny() == w) {
            return edge.getDistance();
        }
    }
    return -1.0;
}



void Graph::optimizedTriangularApproximation() {
    auto startTime = high_resolution_clock::now();
    vector<int> path;
    // create distance matrix
    double avgDist = 0;
    double nEdges = 0;


    avgDist /= nEdges; // the cluster radius is the avg dist
    vector<vector<Vertex>> newAdj(vertexSet.size(), vector<Vertex>()); // cluster adjacency matrix
    bool *visited = new bool[vertexSet.size()];
    for (int i = 0; i < vertexSet.size(); ++i) {
        visited[i] = false;
        vertexSet[i].setDist(INFINITY);
    }
    vector<Vertex> anchors;
    createClusters(avgDist, newAdj, anchors);
    vector<vector<Vertex>> mstAdj(vertexSet.size(), vector<Vertex>()); // mst adjacency matrix
    // prim setup

    for (Vertex &anchor: anchors) {
        getPrimMSTopt( visited, anchor, newAdj,  mstAdj); // fill in adjacency matrix
    }
    fill(visited, visited + vertexSet.size(), false);
    bool allVisited = false;
    Vertex startVertex = anchors[0];
    while (!allVisited) {
        preorderWalkOpt(path, visited, startVertex, mstAdj);
        double bestDist = INFINITY;
        allVisited = true;
        for (Vertex &vertex: anchors) {
            if (!visited[vertex.getId()] && getDistance(vertex.getId(),startVertex.getId()) < bestDist) {
                startVertex = vertex;
                bestDist = getDistance(vertex.getId(),startVertex.getId());
                allVisited = false;
            }
        }
    }
    path.push_back(0);
    double finalCost = 0;
    auto finishTime = high_resolution_clock::now();
    auto duration_ = duration_cast<duration<double>>(finishTime - startTime);

    cout << "-- Clustering Algorithm complete --" << endl;
    cout << "Elapsed time: " << duration_.count() << " seconds" << endl;
    cout << "Path taken: ";

    for (int i = 0; i < path.size(); i++) {
        if (i == path.size() - 1) {
            cout << path[i] << endl;
        } else {
            cout << path[i] << "-->";
            finalCost += getDistance(path[i],path[i + 1]);
        }
    }
    cout << "Minimum cost to travel: " << getMinimumCost(path) << endl;
    delete[] visited;
}



bool Graph::calculatePathCostWithoutCoordinates(vector<int> path, double &minimumCost) {
    for (int i = 0; i < path.size() - 1; i++) {
        double cost = checkDistance(path[i], path[i + 1]);
        if (cost == -1.0) return false;
        minimumCost += cost;
    }
    return true;
}



bool Graph::isCircularPath(vector<int> &path, int startingNode) {
    if (path.size() != vertexSet.size()) {
        return false;
    }
    if (path.front() != startingNode || path.back() != startingNode) {
        return false; // Path doesn't start and end at the specified node
    }
    return true;
}



void Graph::checkCurrentFiles() {
    cout << "Current file(s) being used: " << current_edges_file;
    if (!current_vertexes_file.empty()) {
        cout << " and " << current_vertexes_file << endl;
    } else {
        cout << endl;
    }
}



void Graph::runAllAlgorithms() {
    cout << "---Executing all algorithms---" << endl;
    backtracking();
    triangularApproximation();
    Christofides();
}



void Graph::createClusters(double dist, vector<vector<Vertex>> &newAdj,
                           vector<Vertex> &anchors) {
    anchors.push_back(vertexSet[0]);
    vector<vector<Vertex>> clusters;

    clusters.emplace_back();
    for (Vertex &vertex: vertexSet) {
        bool noAnchors = true;
        double bestDist = INFINITY;
        int bestAnchor = -1;
        for (int i = 0; i < anchors.size(); i++) {
            if (getDistance(anchors[i].getId(),vertex.getId()) < dist) {
                if (getDistance(anchors[i].getId(),vertex.getId()) < bestDist) {
                    bestDist = getDistance(anchors[i].getId(),vertex.getId());
                    bestAnchor = i;
                }
                noAnchors = false;
            }
        }
        if (noAnchors) {
            anchors.push_back(vertex);
            vector<Vertex> toAdd = {vertex};
            clusters.push_back(toAdd);
        } else {
            size_t cluster_size = clusters[bestAnchor].size();
            for (int j = 0; j < cluster_size; j++) {
                newAdj[vertex.getId()].push_back(clusters[bestAnchor][j]);
                newAdj[clusters[bestAnchor][j].getId()].push_back(vertex);
            }
            clusters[bestAnchor].push_back(vertex);
        }
    }
}



void
Graph::getPrimMSTopt( bool *visited, Vertex &start, vector<vector<Vertex>> &clusterAdj,
                     vector<vector<Vertex>> &mstAdj) {

    MutablePriorityQueue<Vertex> q;

    visited[start.getId()] = true;
    q.insert(&start);
    start.setDist(0);
    while (!q.empty()) {
        Vertex *vertex = q.extractMin();
        visited[vertex->getId()] = true;
        for (Vertex &next: clusterAdj[vertex->getId()]) {

            if (!visited[next.getId()]) {
                q.insert(&next);
                if (getDistance(vertex->getId(),next.getId())< next.getDist()) {
                    next.setDist(getDistance(vertex->getId(),next.getId()));
                    mstAdj[vertex->getId()].push_back(next);
                    q.decreaseKey(&next);
                }
            }
        }
    }
}



void Graph::preorderWalkOpt(vector<int> &path, bool *visited, Vertex &start, vector<vector<Vertex>> &mstAdj) {
    visited[start.getId()] = true;
    path.push_back(start.getId());
    for (Vertex &vertex: mstAdj[start.getId()]) {
        if (!visited[vertex.getId()]) {
            preorderWalkOpt(path, visited, vertex, mstAdj);
        }
    }
}
