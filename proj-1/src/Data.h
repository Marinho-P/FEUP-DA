#ifndef DATA_H
#define DATA_H

#define CITIES "../dataset/Cities.csv"
#define PIPES "../dataset/Pipes.csv"
#define RESERVOIRS "../dataset/Reservoir.csv"
#define STATIONS "../dataset/Stations.csv"

#define CITIES_MADEIRA "../smallDataSet/Cities_Madeira.csv"
#define PIPES_MADEIRA "../smallDataSet/Pipes_Madeira.csv"
#define RESERVOIRS_MADEIRA "../smallDataSet/Reservoirs_Madeira.csv"
#define STATIONS_MADEIRA "../smallDataSet/Stations_Madeira.csv"


#include <unordered_map>
#include "NodeData.h"
#include "City.h"
#include "Reservoir.h"
#include "Pipe.h"
#include "Station.h"
#include "data_structures/Graph.h"
#include <unordered_set>


/** @brief Class where all classes interact and where system requests are handled */
class Data {
public:
    /** @brief Constructor for the Data class */
    Data();
    /**
     * @brief Read data from the "cities.csv" / "cities_madeira.csv" file and populate City objects.
     * This function reads data from the "cities.csv" / "cities_madeira.csv" file, creates City objects for each entry,
     * and populates the corresponding data structures in the Data class.
     * @details The time complexity of this function depends on the number of entries in the "cities.csv" / "cities_madeira.csv" file.
     * Let 'n' be the number of lines in the file. Since each line is processed sequentially, the time complexity
     * is O(n), assuming constant time operations for each line read and processing.
     */
    void read_cities();
    /**
     * @brief Read data from the "pipes.csv" / "pipes_madeira.csv" file and populate Pipe objects.
     * This function reads data from the "pipes.csv" / "pipes_madeira.csv" file, creates Pipe objects for each entry,
     * and populates the corresponding data structures in the Data class.
     * @details The time complexity of this function depends on the number of entries in the "pipes.csv" / "pipes_madeira.csv" file.
     * Let 'n' be the number of lines in the file. Since each line is processed sequentially, the time complexity
     * is O(n), assuming constant time operations for each line read and processing.
     */
    void read_pipes();
    /**
     * @brief Read data from the "reservoirs.csv" / "reservoirs_madeira.csv" file and populate Reservoir objects.
     * This function reads data from the "reservoirs.csv" / "reservoirs_madeira.csv" file, creates Reservoir objects for each entry,
     * and populates the corresponding data structures in the Data class.
     * @details The time complexity of this function depends on the number of entries in the "reservoirs.csv" / "reservoirs_madeira.csv" file.
     * Let 'n' be the number of lines in the file. Since each line is processed sequentially, the time complexity
     * is O(n), assuming constant time operations for each line read and processing.
     */
    void read_reservoir();
    /**
     * @brief Read data from the "stations.csv" / "stations_madeira.csv" file and populate Station objects.
     * This function reads data from the "stations.csv" / "stations_madeira.csv" file, creates Station objects for each entry,
     * and populates the corresponding data structures in the Data class.
     * @details The time complexity of this function depends on the number of entries in the "stations.csv" / "stations_madeira.csv" file.
     * Let 'n' be the number of lines in the file. Since each line is processed sequentially, the time complexity
     * is O(n), assuming constant time operations for each line read and processing.
     */
    void read_stations();
    /**
     * @brief Checks and prints the maximum water flow to a specific city.
     * @details This function calculates the maximum flow of water to a specific city in the network using the Edmonds-Karp algorithm.
     * The complexity of this algorithm is O(V * E^2), where V is the number of vertices in the graph and E is the number of edges.
     */
    void checkMaxWaterCity();
    /**
     * @brief Checks and prints the cities with water deficit.
     * This function calculates the water deficit for each city in the network by comparing the demand of each city with the actual water flow reaching it.
     * It iterates over each city vertex in the graph and computes the flow of water reaching the city. If the flow is less than the city's demand, it indicates a deficit.
     * @details The complexity of this function depends on the number of cities and the number of incoming edges to each city vertex. Let n be the number of cities and m be the average number of incoming edges per city vertex.
     * In the worst case scenario, it iterates over all city vertices and their incoming edges, resulting in a time complexity of O(n * m).
     */
    void checkCitiesWaterDeficit();
    /**
     * @brief Performs a deep copy of the graph.
     * This function deep copies the graph, including all its vertices and edges.
     * It iterates through the vertices of the original graph and creates deep copies of them,
     * adding them to the new graph. Then, it iterates through the edges of each vertex,
     * creating new edges in the new graph while maintaining their weights, flows, and reverses.
     * @param graph = A pointer to the original graph to be copied.
     * @param oldmap = A map that maps the codes of vertices in the original graph to the vertices themselves.
     * @param newmap = A map used to map the codes of new (copied) vertices to the vertices themselves.
     */
    void deepCopyGraph(Graph<NodeData*> *graph, unordered_map<string, Vertex<NodeData*>*> oldmap, unordered_map<string, Vertex<NodeData*>*> &newmap);
    /** @brief Creates a modified graph for the Edmonds-Karp algorithm and performs the maximum flow calculation.*/
    void createEdmondskarpG();
    /**
     * @brief Adds a super source and super sink to the given graph and updates the capacities accordingly.
     * @param graph = A pointer to the graph.
     * @param map = A reference to the map containing vertex mappings.
     */
    void addSuperSource_Sink(Graph<NodeData*> *graph, unordered_map<string, Vertex<NodeData*>*> &map);
    /**
     * @brief Checks and prints the maximum water flow in the entire network.
     * @details This function calculates the maximum flow of water in the entire network using the Edmonds-Karp algorithm.
     * Therefore the complexity of this algorithm is O(V * E^2), where V is the number of vertices in the graph and E is the number of edges.
     */
    void checkMaxWaterWholeNetwork();
    /**
     * @brief Compares statistics before and after balancing the pipe network.
     * This function prints statistics about the difference between pipe capacity and flow before and after
     * applying a balancing algorithm to the pipe network.
     */
    void checkBefore_AfterBalancing();
    /**
     * @brief Calculates and prints statistics about the difference between pipe capacity and flow.
     * This function calculates statistics including the average difference, variance, and maximum difference
     * between the capacity and flow of pipes in the given graph, excluding the superSource and superSink nodes
     * from the calculation.
     * @param graph = a pointer to the graph containing the pipe network.
     * @param nPipes = the total number of pipes in the network.
     */
    void print_calculateStatistics(Graph<NodeData*> *graph, double nPipes);
    /**
     * @brief Applies a pipe balancing algorithm on the graph.
     * This method is responsible for implementing a pipe balancing algorithm on the provided graph.
     * @param pGraph = the graph on which the pipe balancing will be applied.
     * @return The graph after applying the balancing algorithm.
     */
    Graph<NodeData*> * balancePipes(Graph<NodeData*> *pGraph);
    /**
     * @brief Determines and prints the cities affected by pipeline rupture.
     * This function simulates the effect of a pipeline rupture on the network by temporarily removing the specified pipeline(s) from the graph.
     * It then recalculates the flow of water in the network using the Edmonds-Karp algorithm and checks for any decrease in water flow to cities.
     * @details The time complexity of this function depends on the number of pipelines to be ruptured (let's denote it as k) and the complexity of the Edmonds-Karp algorithm, which is O(V * E^2), where V is the number of vertices and E is the number of edges in the graph.
     * Removing k pipelines from the graph and recalculating the flow using the Edmonds-Karp algorithm for each pipeline rupture takes O(k * (V * E^2)) time.
     */
    void citiesAffectedPipelineRupture();
    /**
     * @brief Determines and prints the cities affected by the removal of a pumping station.
     * This function simulates the effect of removing a pumping station from the network by temporarily removing the station from the graph.
     * It then recalculates the flow of water in the network using the Edmonds-Karp algorithm and checks for any decrease in water flow to cities.
     * @details The time complexity of this function depends on the number of cities (n) and the complexity of the Edmonds-Karp algorithm, which is O(V * E^2), where V is the number of vertices and E is the number of edges in the graph.
     * Removing a pumping station from the graph and recalculating the flow using the Edmonds-Karp algorithm takes O(n * (V * E^2)) time.
     */
    void citiesAffectedPumpingStationRemoval();
    /**
     * @brief Determines and prints the cities affected by the removal of a water reservoir.
     * This function simulates the effect of removing a water reservoir from the network by temporarily removing the reservoir vertex from the graph.
     * It then recalculates the flow of water in the network using the Edmonds-Karp algorithm and checks for any decrease in water flow to cities.
     * @details The time complexity of this function depends on the number of cities (n) and the complexity of the Edmonds-Karp algorithm, which is O(V * E^2), where V is the number of vertices and E is the number of edges in the graph.
     * Removing a reservoir from the graph and recalculating the flow using the Edmonds-Karp algorithm takes O(n * (V * E^2)) time.
     */
    void citiesAffectedWaterReservoirRemoval();
    /**
     * @brief Determines and prints the cities affected by any pipeline rupture specified by the user.
     * This function simulates the effect of pipeline rupture(s) by temporarily removing the affected pipeline(s) from the graph.
     * It then recalculates the flow of water in the network using the Edmonds-Karp algorithm and checks for any decrease in water flow to cities.
     * @details The time complexity of this function depends on the number of cities (n), the number of specified pipelines to rupture (m), and the complexity of the Edmonds-Karp algorithm, which is O(V * E^2), where V is the number of vertices and E is the number of edges in the graph.
     * Removing pipelines from the graph and recalculating the flow using the Edmonds-Karp algorithm takes O(m * (V * E^2)) time.
     */
    void citiesAffectedANYPipelineRupture();
    /** @brief Cleans up resources and deallocates memory.*/
    void cleanUp();

private:
    Graph<NodeData*>* waterG;
    Graph<NodeData*>* edmondskarpG;
    unordered_map<string,Vertex<NodeData*>*> nodesKarpG;
    unordered_map<string,Vertex<NodeData*>*> nodes;
    unordered_map<int,Vertex<NodeData*>*> reservoirs;
    unordered_map<int,Vertex<NodeData*>*> cities;
    unordered_map<int,Vertex<NodeData*>*> stations;
    unordered_map<string,Pipe*> pipes;
    /**
     * @brief Constructs and returns a vector with all the edges of a path.
     * During the balancing algorithm, the path variable on all vertices is set to the edge used to get to a vertex, the algorithm uses them to construct a path.
     * @details The time complexity of this function depends on the number of edges ( E ) in the path being constructed and it is O( E ) as it has to go through them all to constructed the complete path.
     * @param pVertex the last vertex in the path.
     * @return a vector with all the edges in the path.
     */
    vector<Edge<NodeData *> *> pathWalkback(Vertex<NodeData *> *pVertex);
    /**
     * @brief Finds the "longest" augmenting path in the graph.
     * Not actually the longest as it would be O(2^N) to calculate the longest trail, instead it calculates the farthest city from any reservoir.
     * @details The time complexity depends on the number of reservoirs ( R ), the number of edges ( E ) and the number of vertices ( V ) and it is O( R*(V+E)) as, for each reservoir it has to go through the graph, in some cases, in its entirety.
     * @param graph The graph being used
     * @param endPathNode The last vertex of the chosen path
     * @param flowToAdd The flow to increase in the chosen path
     * @param nodeMap A map with all the nodes of the graph ( for ease of access )
     */
    vector<Edge<NodeData *> *>
    findLongestPath(Graph<NodeData *> *graph, unordered_map<string, Vertex<NodeData *> *> nodeMap, Vertex<NodeData *> *&endPathNode, double &flowToAdd);
    /**

@brief Augments the flow along the longest path in the graph.
This function augments the flow along the longest path in the graph based on the calculated flow to add.
@param vector1 = a vector containing edges of the longest path.
@param pVertex = the end vertex of the longest path.
@param flow = the amount of flow to add to the path.
@details The time complexity of this function is O(E), where E is the number of edges in the longest path.
*/
    void augmentLongestPath(vector<Edge<NodeData *> *> vector1, Vertex<NodeData *> *pVertex, double flow);
};


#endif