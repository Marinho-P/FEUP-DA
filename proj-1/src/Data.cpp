#include <fstream>
#include <sstream>
#include <cmath>
#include "Data.h"
#include "data_structures/Graph.h"
#include "NodeData.h"


Data::Data() {
    waterG = new Graph<NodeData*>();
    read_cities();
    read_stations();
    read_reservoir();
    read_pipes();
    createEdmondskarpG();
}


void Data::read_cities() {
    string name,code;
    int population,id;
    double demand;
    std::ifstream file(CITIES);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            istringstream iss(line);
            getline(iss,name,',');
            string id_str,population_str,demand_str;
            getline(iss,id_str,',');
            getline(iss,code,',');
            getline(iss,demand_str,',');
            getline(iss,population_str,',');
            id = stoi(id_str);
            population = stoi(population_str);
            demand = stod(demand_str);
            City *city = new City(name,id,code,demand,population);
            Vertex<NodeData*>* l = waterG->addReturnVertex(city);
            cities[id] = l;
            nodes[code] = l;
        }
    }else {
        cout << "Could not open the file\n";
    }
}


void Data::read_pipes() {
    bool direction;
    string pointA,pointB;
    int capacity;
    int id = 1;
    std::ifstream file(PIPES);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string capacity_str,direction_str;
            istringstream iss(line);
            getline(iss,pointA,',');
            getline(iss,pointB,',');
            getline(iss,capacity_str,',');
            getline(iss,direction_str,',');
            direction = stoi(direction_str);
            capacity = stoi(capacity_str);
            Pipe* pipe = new Pipe(pointA,pointB,capacity,direction);
            pipes[pointA+"-"+pointB] = pipe;
            if(direction){
                waterG->addEdge(nodes[pointA]->getInfo(),nodes[pointB]->getInfo(),capacity);
            }else{
                waterG->addBidirectionalEdge(nodes[pointA]->getInfo(),nodes[pointB]->getInfo(),capacity);
            }
            id++;
        }
    }else {
        cout << "Could not open the file\n";
    }
}


void Data::read_reservoir() {
    string name,code,municipality;
    int id,maxDelivery;
    std::ifstream file(RESERVOIRS);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string id_str,maxDelivery_str;
            istringstream iss(line);
            getline(iss,name,',');
            getline(iss,municipality,',');
            getline(iss,id_str,',');
            getline(iss,code,',');
            getline(iss,maxDelivery_str,',');
            id = stoi(id_str);
            maxDelivery = stoi(maxDelivery_str);
            Reservoir* reservoir = new Reservoir(name,municipality,id,code,maxDelivery);
            Vertex<NodeData*>* l = waterG->addReturnVertex(reservoir);
            reservoirs[id] = l;
            nodes[code] = l;
        }
    }else {
        cout << "Could not open the file\n";
    }
}


void Data::read_stations() {
    string code;
    int id;
    std::ifstream file(STATIONS);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string id_str;
            istringstream iss(line);
            getline(iss,id_str,',');
            getline(iss,code,',');
            id = stoi(id_str);
            Station* station = new Station(id,code);
            Vertex<NodeData*>* l = waterG->addReturnVertex(station);
            nodes[code] = l;
            stations[id] = l;
        }
    }else {
        cout << "Could not open the file\n";
    }
}


/** @brief Resets the flow in the given graph to zero.*/
void resetFlow(const Graph<NodeData*>* g) {
    for( Vertex<NodeData*>* v:g->getVertexSet()){
        for(Edge<NodeData*>* edge: v->getAdj()){
            edge->setFlow(0);
        }
    }
}


void Data::deepCopyGraph(Graph<NodeData*>* graph, unordered_map<string,Vertex<NodeData*>*> oldmap, unordered_map<string,Vertex<NodeData*>*> &newmap){
    for(auto pair:oldmap){
        auto v = pair.second;
        NodeData* nodeData;
        switch (v->getInfo()->getType()) {
            case CITY: {
                City *cityNode = (City *) v->getInfo();
                nodeData = new City(cityNode->getName(), cityNode->getId(), cityNode->getCode(), cityNode->getDemand(),
                                    cityNode->getPopulation());
                break;
            }
            case RESERVOIR: {
                Reservoir *reservoirNode = (Reservoir *) v->getInfo();
                nodeData = new Reservoir(reservoirNode->getName(), reservoirNode->getMunicipality(),
                                         reservoirNode->getId(), reservoirNode->getCode(),
                                         reservoirNode->getMaxDelivery());
                break;
            }
            case STATION: {
                Station *stationNode = (Station *) v->getInfo();
                nodeData = new Station(stationNode->getId(), stationNode->getCode());
                break;
            }
            default:
                cout << "Invalid type" << endl;
                break;
        }
        Vertex<NodeData*>* vertex = graph->addReturnVertex(nodeData);
        newmap[v->getInfo()->getCode()] =  vertex;
    }
    for(auto pair:oldmap){
        Vertex<NodeData*>* v = pair.second;
        for(auto edge: v->getAdj()){
            Edge<NodeData*>* newEdge = newmap[v->getInfo()->getCode()]->addEdge(newmap[edge->getDest()->getInfo()->getCode()],edge->getWeight());
            newEdge->setFlow(edge->getFlow());
            newEdge->setReverse(edge->getReverse());
        }
    }
}


/**
 * @brief Function to test the given vertex 'w' and visit it if conditions are met.
 * @tparam T = the type of data stored in the vertex and edge
 * @param q = a reference to the queue used for BFS traversal
 * @param e = a pointer to the edge leading to vertex 'w'
 * @param w = a pointer to the vertex 'w' to be tested and possibly visited
 * @param residual = the residual capacity along the edge 'e' leading to vertex 'w'
 */
template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
    // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}


/**
 * @brief Function to find an augmenting path using Breadth-First Search
 * @tparam T = the type of data stored in the graph
 * @param g = a pointer to the graph
 * @param s = a pointer to the source vertex
 * @param t = a pointer to the target vertex
 * @return true if an augmenting path is found, false otherwise.
 */
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    // Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
    }
    // Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<T> *> q;
    q.push(s);
    // BFS to find an augmenting path
    while (! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
        // Process outgoing edges
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        // Process incoming edges
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}


/**
 * @brief Function to find the minimum residual capacity along the augmenting path
 * @tparam T = the type of data stored in the graph
 * @param s = a pointer to the source vertex
 * @param t = a pointer to the target vertex
 * @return The minimum residual capacity along the augmenting path.
 */
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INFINITY;
    // Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    // Return the minimum residual capacity
    return f;
}


/**
 * @brief Function to augment flow along the augmenting path with the given flow value
 * @tparam T = the type of data stored in the graph
 * @param s = a pointer to the source vertex
 * @param t = a pointer to the target vertex
 * @param f = the flow value to be augmented along the path
 */
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    // Traverse the augmenting path and update the flow values accordingly
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}


/**
 * @brief Main function implementing the Edmonds-Karp algorithm for maximum flow in a graph.
 * @tparam T = the type of data stored in the graph.
 * @param g = a pointer to the graph.
 * @param source = a pointer to the source node.
 * @param target = a pointer to the target node.
 */
template <class T>
void edmondsKarp(Graph<T> *g, NodeData* source, NodeData* target) {
    // Find source and target vertices in the graph
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);
    // Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t) {
        throw std::logic_error("Invalid source and/or target vertex");
    }
    // While there is an augmenting path, augment the flow along the path
    while (findAugmentingPath(g, s, t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}


void Data::createEdmondskarpG(){
    edmondskarpG = new Graph<NodeData*>();
    deepCopyGraph(edmondskarpG,nodes,nodesKarpG);
    addSuperSource_Sink(edmondskarpG,nodesKarpG);
    resetFlow(edmondskarpG);
    edmondsKarp(edmondskarpG,nodesKarpG["superSource"]->getInfo(),nodesKarpG["superSink"]->getInfo());
}


void Data::addSuperSource_Sink(Graph<NodeData*>* graph, unordered_map<string,Vertex<NodeData*>*> &map){
    Reservoir* source = new Reservoir("superSource","superSource",-1,"superSource",INFINITY);
    Vertex<NodeData*>* vertex = graph->addReturnVertex(source);
    // Add new source to Map
    map[vertex->getInfo()->getCode()] = vertex;
    // Set the capacity of the SuperSource->Reservoir MaxDelivery of Reservoir
    for(auto pair:reservoirs){
        auto w = pair.second;
        Reservoir* reservoir = (Reservoir*) w->getInfo();
        vertex->addEdge(graph->findVertex(map[w->getInfo()->getCode()]->getInfo()),reservoir->getMaxDelivery());
    }
    // SuperSink
    City* sink = new City("superSink",-1,"superSink",INFINITY,0);
    Vertex<NodeData*>* sinkVertex = graph->addReturnVertex(sink);
    map[sinkVertex->getInfo()->getCode()] = sinkVertex;
    for(auto pair:cities){
        auto w = pair.second;
        City* pcity = (City*)w->getInfo();
        graph->findVertex(map[w->getInfo()->getCode()]->getInfo())->addEdge(sinkVertex,pcity->getDemand());
    }
}


//T2.1
void Data::checkMaxWaterWholeNetwork() {
    Vertex<NodeData*>* node = edmondskarpG->findVertex(nodesKarpG["superSink"]->getInfo());
    int maxFlow = 0;
    for(auto edge:node->getIncoming()){
        maxFlow+=edge->getFlow();
    }
    cout << "The Max flow of water in the whole network is: " << maxFlow <<  endl;
    cout << "\n";
}


//T2.1
void Data::checkMaxWaterCity(){
    cout << "Type the id of the respective city:";
    int id;
    cin >> id;
    if(id > cities.size() || id < 1){
        cout << "There is no city with such id" << endl;
        cout << "\n";
        return;
    }
    string code = "C_"+ to_string(id);
    Vertex<NodeData*>* node = edmondskarpG->findVertex(nodesKarpG[code]->getInfo());
    int maxFlow = 0;
    for(auto edge:node->getIncoming()){
        maxFlow += edge->getFlow();
    }
    City* pCity = (City*) node->getInfo();
    cout << "Max water to arrive to city, " << pCity->getName() << " with code, "<< pCity->getCode() << ", is " << maxFlow <<  endl;
    cout << "\n";
}


void Data::print_calculateStatistics(Graph<NodeData*>* graph,double nPipes){
    double average = 0;
    double maxdif = 0;
    double mindif = INFINITY;
    for(auto v:graph->getVertexSet()){
        if(v->getInfo()->getCode() == "superSource" || v->getInfo()->getCode() == "superSink"){
            continue;
        }
        for(auto e:v->getAdj()){
            if(e->getDest()->getInfo()->getCode() == "superSource" || e->getDest()->getInfo()->getCode() == "superSink" ){
                continue;
            }
            double difference = (e->getWeight() - e->getFlow());
            if(maxdif < difference){
                maxdif = difference;
            }
            if(mindif > difference){
                mindif = difference;
            }
            average+= difference;
        }
    }
    average/=nPipes;
    double variance = 0;
    for( auto v: graph->getVertexSet() ){
        if(v->getInfo()->getCode() == "superSource" || v->getInfo()->getCode() == "superSink"){
            continue;
        }
        for(auto e:v->getAdj()){
            variance += (e->getFlow() -average)*(e->getFlow()- average);
        }
    }
    variance/= nPipes;
    cout << "The average difference of capacity-flow is " << average << ", having variance of " << variance << " and max difference " << maxdif << endl;
}


//T2.3
void Data::checkBefore_AfterBalancing(){
    cout << "---Before Balancing---" << endl;
    print_calculateStatistics(edmondskarpG,pipes.size());
    Graph<NodeData*>* balenced = balancePipes(edmondskarpG);

    cout << "---After using the Balancing algorithm---" << endl;
    print_calculateStatistics(balenced,pipes.size());
    cout << "\n";
    delete balenced;
}


//T2.2
void Data::checkCitiesWaterDeficit() {
    stringstream stream;
    for(pair<int,Vertex<NodeData*>*> tuple: cities){
        Vertex<NodeData*>* city = nodesKarpG[tuple.second->getInfo()->getCode()];
        int flow = 0;
        NodeData* node = city->getInfo();
        for (Edge<NodeData*>* edge: city->getIncoming()){
            flow += edge->getFlow();
        }
        City* pCity = (City*) node;
        if(flow <  pCity->getDemand()){
            stream << "City:" << pCity->getCode() << "-" << pCity->getName() << " | Actual flow:" << flow << " | Demand:" << pCity->getDemand()<< " | Deficit:"  << pCity->getDemand() - flow << endl;
        }
    }
    if(stream.str().empty()){
        cout << "There are no cities that fail to meet the demand" << endl;
        cout << "\n";
    }else{
        cout << stream.str();
        cout << "\n";
    }
}
/**
* @brief Debug function for testing if any of the flow rules are broken.
* This functions tests if the flow entering a pumping station is the same as the one exiting, whether a reservoir is sending more water than its maxDelivery and whether a city is receiving more water than its Demand
* @details The time complexity of this function depends on the number of vertices (V) and the number of edges (E) and it is O( V + E ) as it has to go through all vertices and edges in the graph.
 * @param graph The graph to be analyzed.
*/
void flowRules(Graph<NodeData*>* graph){
    bool pass = true;
    for(auto v: graph->getVertexSet()){
        double reservoirFlow = 0;
        if(v->getInfo()->getType() == RESERVOIR){
            for(auto e:v->getAdj()){
                reservoirFlow += e->getFlow();
            }
            Reservoir* reservoir = (Reservoir*) v->getInfo();
            if(reservoir->getMaxDelivery() < reservoirFlow){
                cout << reservoir->getCode() << ": max delivery exceeded" << endl;
                pass = false;
            }
            if(reservoir->getMaxDelivery() > reservoirFlow){
                cout << reservoir->getCode() << ": could add more water" << endl;
            }
        }
    }
    for(auto v: graph->getVertexSet()){
        if(v->getInfo()->getType() != STATION){
            continue;
        }
        double inFlow = 0;
        double outFlow = 0;
        for(auto e: v->getAdj()){
            if(e->getFlow() > e->getWeight()){
                cout << "Edge is breaking the capacity rule" << endl;
                pass = false;
            }
            outFlow += e->getFlow();
        }
        for(auto e: v->getIncoming()){
            if(e->getFlow() > e->getWeight()){
                cout << "Edge is breaking the capacity rule" << endl;
                pass = false;
            }
            inFlow += e->getFlow();
        }
        if(outFlow != inFlow){
            cout << v->getInfo()->getCode() << "failed the flow conservation rule" << endl;
            pass = false;
        }
    }
    for(auto v: graph->getVertexSet()){
        double cityFlow = 0;
        if(v->getInfo()->getType() == CITY){
            for(auto e:v->getAdj()){
                cityFlow += e->getFlow();
            }
            City* city = (City*) v->getInfo();
            if(city->getDemand()< cityFlow){
                cout << city->getCode() << ": demand surplus" << endl;
                pass = false;
            }
            if(city->getDemand()> cityFlow){
                cout << city->getCode() << ": could add more water" << endl;
            }
        }
    }
    if(pass){
        cout << "All good!" << endl;
    }
}


Graph<NodeData *> * Data::balancePipes(Graph<NodeData *> *pGraph) {
    Graph<NodeData*>* balanced = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(balanced,nodes,nodeMap);
    vector<Edge<NodeData*>*> path;
    Vertex<NodeData*>* endOfPath;
    double flowToAdd;
    path = findLongestPath(balanced,nodeMap,endOfPath,flowToAdd);
    while(  !path.empty()){
        augmentLongestPath(path,endOfPath,flowToAdd);
        path = findLongestPath(balanced,nodeMap,endOfPath,flowToAdd);
    }
    double maxFlow = 0;
    for(auto tuple:cities){
        auto city = nodeMap[tuple.second->getInfo()->getCode()];
        for(auto e: city->getIncoming()){
            maxFlow += e->getFlow();
        }
    }
    //cout << "Max flow:" << maxFlow << endl;
    //flowRules(balanced);
    return balanced;
}


vector<Edge<NodeData*>*> Data:: findLongestPath(Graph<NodeData*>* graph,unordered_map<string,Vertex<NodeData*>*> nodeMap,Vertex<NodeData*>*& endPathNode,double& flowToAdd){
    flowToAdd = 0;
    queue<pair<Vertex<NodeData*>*,double>> q;
    Vertex<NodeData*>* finalCity;
    vector<Edge<NodeData*>*> path;
    double maxDist = numeric_limits<double>::min();
    for(auto tuple: reservoirs){
        for(auto v: graph->getVertexSet()){
            v->setVisited(false);
            v->setPath(nullptr);
            v->setDist(numeric_limits<double>::min());
        }
        auto v = tuple.second;
        Vertex<NodeData*>* reservoir = nodeMap[v->getInfo()->getCode()];
        double deliveryConsumed = 0;
        for(auto e: reservoir->getAdj()){
            deliveryConsumed += e->getFlow();
        }
        Reservoir* reservoirInfo = (Reservoir*) reservoir->getInfo();
        reservoir->setVisited(true);
        reservoir->setDist(0);
        if(reservoirInfo->getMaxDelivery() - deliveryConsumed == 0){
            continue;
        }
        q.push({reservoir,reservoirInfo->getMaxDelivery() - deliveryConsumed });
        while( !q.empty()){
            auto current = q.front().first;
            auto currentFlow = q.front().second;
            q.pop();
            if( current->getInfo()->getType() == CITY && (current->getDist() >maxDist || (current->getDist() == maxDist && currentFlow > flowToAdd) )){
                double demandAccomplished = 0;
                for(auto e: current->getIncoming()){
                    demandAccomplished += e->getFlow();
                }
                City* city = (City*) current->getInfo();
                if(city->getDemand()- demandAccomplished == 0){
                    continue;
                }
                flowToAdd = min(currentFlow,city->getDemand()- demandAccomplished);
                finalCity = current;
                maxDist = current->getDist();
                path = pathWalkback(current);
                continue;
            }
            // outgoing edges
            for(auto e: current->getAdj()){
                if(!e->getDest()->isVisited() && e->getFlow() < e->getWeight()){
                    q.push({e->getDest(),min(e->getWeight()-e->getFlow(),currentFlow)});
                    e->getDest()->setPath(e);
                    e->getDest()->setDist(current->getDist()+1);
                    e->getDest()->setVisited(true);
                }
            }
            for(auto e: current->getIncoming()){
                if(!e->getDest()->isVisited() && e->getFlow() > 0){
                    q.push({e->getOrig(),min(e->getFlow(),currentFlow)});
                    e->getOrig()->setPath(e);
                    e->getOrig()->setVisited(true);
                    // might be wrong
                    if(e->getReverse() != nullptr && currentFlow > e->getFlow()){
                        e->getOrig()->setDist(current->getDist() + 1);
                    }
                    else{
                        e->getOrig()->setDist(current->getDist() - 1);
                    }
                }
            }
        }
    }
    endPathNode = finalCity;
    return path;
}


vector<Edge<NodeData*>*> Data::pathWalkback(Vertex<NodeData *> *pVertex) {
    vector<Edge<NodeData*>*> path;
    while(pVertex->getInfo()->getType() != RESERVOIR){
        path.push_back(pVertex->getPath());
        //normal edge
        if(pVertex->getPath()->getDest() == pVertex){
            pVertex = pVertex->getPath()->getOrig();
        }
            // reverse edge
        else{
            pVertex = pVertex->getPath()->getDest();
        }
    }
    return path;
}


void Data::augmentLongestPath(vector<Edge<NodeData *> *> vector1, Vertex<NodeData *> *pVertex,double flow) {
    for (auto edge: vector1) {
        if (edge->getDest() == pVertex) {
            edge->setFlow(edge->getFlow() + flow);
            pVertex = edge->getOrig();
        } else {
            // might be wrong
            if (flow > edge->getFlow()) {
                edge->getReverse()->setFlow(flow - edge->getFlow());
                edge->setFlow(0);

            } else {
                edge->setFlow(edge->getFlow() - flow);
            }
            pVertex = edge->getDest();
        }
    }
}


//T3.3
void Data::citiesAffectedPipelineRupture(){
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    stringstream stream;
    for(auto pipe:pipes){
        if(pipe.second->isDirection()){
            graph->removeEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo());
        }else{
            graph->removeEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo());
            graph->removeEdge(nodeMap[pipe.second->getServicePointB()]->getInfo(),nodeMap[pipe.second->getServicePointA()]->getInfo());
        }
        resetFlow(graph);
        edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
        for(auto city:cities){
            string code = city.second->getInfo()->getCode();
            City* pcity = (City*)nodeMap[code]->getInfo();
            int flow = 0;
            int oldFlow = 0;
            for(auto e:nodeMap[code]->getIncoming()){
                flow+=e->getFlow();
            }
            for(auto e:nodesKarpG[code]->getIncoming()){
                oldFlow+=e->getFlow();
            }
            if(flow < oldFlow){
                stream << "/* Pipe rupture from " << pipe.second->getServicePointA() << "--" << pipe.second->getServicePointB() << " */" <<endl;
                stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << oldFlow - flow << endl;
            }
        }
        if(pipe.second->isDirection()){
            graph->addEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo(),pipe.second->getCapacity());
        }
        else{
            graph->addBidirectionalEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo(),pipe.second->getCapacity());
        }
    }
    if(stream.str().empty()){
        cout << "There is no city affected by this pipe" << endl;
        cout << "\n";
    }else{
        cout << stream.str();
        cout << "\n";
    }
    delete graph;
}


//T3.1
void Data::citiesAffectedWaterReservoirRemoval(){
    cout << "Type the id of the reservoir you want to remove:";
    int resevoirID;
    cin >> resevoirID;
    cout << endl;
    if(resevoirID > reservoirs.size() || resevoirID < 1){
        cout << "There is no reservoir with such id" << endl;
        cout << "\n";
        return;
    }
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    stringstream stream;
    Vertex<NodeData*>* oldVertex = nodeMap["R_"+ to_string(resevoirID)];
    NodeData* newVertexInfo = new Reservoir(*(Reservoir*)oldVertex->getInfo());
    graph->removeVertex(oldVertex->getInfo());
    resetFlow(graph);
    edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
    for(auto city:cities){
        string code = city.second->getInfo()->getCode();
        City* pcity = (City*)nodeMap[code]->getInfo();
        int flow = 0;
        int oldFlow = 0;
        for(auto e:nodeMap[code]->getIncoming()){
            flow+=e->getFlow();
        }
        for(auto e:nodesKarpG[code]->getIncoming()){
            oldFlow+=e->getFlow();
        }
        Reservoir* preservoir = (Reservoir*)newVertexInfo;
        if(flow < oldFlow){
            stream << "/* Reservoir out of service " << preservoir->getName() << " with code " << preservoir->getCode() << " */"<< endl;
            stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << oldFlow - flow << endl;
        }
    }
    if(stream.str().empty()){
        cout << "There is no city affected by the removal of this reservoir" << endl;
        cout << "\n";
    }else{
        cout << stream.str();
        cout << "\n";
    }
    delete graph;
    delete newVertexInfo;
}


//T3.2
void Data::citiesAffectedPumpingStationRemoval(){
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    stringstream stream;
    for(auto station:stations){
        Vertex<NodeData*>* oldVertex = nodeMap[station.second->getInfo()->getCode()];
        NodeData* newVertexInfo = new Station(oldVertex->getInfo()->getId(),oldVertex->getInfo()->getCode());
        vector<pair<Vertex<NodeData*>*,double>> incoming;
        vector<pair<Vertex<NodeData*>*,double>> adj;
        for(auto e:oldVertex->getIncoming()){
            incoming.push_back({e->getOrig(),e->getWeight()});
        }
        for(auto e:oldVertex->getAdj()){
            adj.push_back({e->getDest(),e->getWeight()});
        }
        graph->removeVertex(oldVertex->getInfo());
        resetFlow(graph);
        edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
        for(auto city:cities){
            string code = city.second->getInfo()->getCode();
            City* pcity = (City*)nodeMap[code]->getInfo();
            int flow = 0;
            int oldFlow = 0;
            for(auto e:nodeMap[code]->getIncoming()){
                flow += e->getFlow();
            }
            for(auto e:nodesKarpG[code]->getIncoming()){
                oldFlow += e->getFlow();
            }
            if(flow < oldFlow){
                stream << "/* Station out of service " << station.second->getInfo()->getCode() << " */"<< endl;
                stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << oldFlow - flow << endl;
            }
        }
        // Reset Pumping Station
        Vertex<NodeData*>* node = graph->addReturnVertex(newVertexInfo);
        for(auto inc:incoming){
            inc.first->addEdge(node,inc.second);
        }
        for(auto a:adj){
            node->addEdge(a.first,a.second);
        }
    }
    if(stream.str().empty()){
        cout << "There is no city affected by the removal of any station" << endl;
        cout << "\n";
    }else{
        cout << stream.str();
        cout << "\n";
    }
    delete graph;
}


//T3.3
void Data::citiesAffectedANYPipelineRupture(){
    unordered_set<string> pipelines;
    string input;
    std::cout << "Enter the pipeline codes in the format codeA-codeB. Type 'exit' to stop: " << endl;
    while (std::cin >> input && input != "exit") {
        pipelines.insert(input);
    }
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    stringstream stream;
    stringstream pipeStream;
    bool first_iter = true;
    for(auto codes:pipelines){
        auto pipe = pipes[codes];
        if(pipe == nullptr){
            string reverseCode = codes.substr(codes.find("-")+1) + "-" + codes.substr(0,codes.find("-"));
            auto reversePipe = pipes[reverseCode];
            if(reversePipe != nullptr){
                if(!reversePipe->isDirection()){
                    graph->removeEdge(nodeMap[reversePipe->getServicePointA()]->getInfo(),nodeMap[reversePipe->getServicePointB()]->getInfo());
                    graph->removeEdge(nodeMap[reversePipe->getServicePointB()]->getInfo(),nodeMap[reversePipe->getServicePointA()]->getInfo());
                    if(first_iter){
                        pipeStream << "/* Pipe rupture(s) from " << reversePipe->getServicePointB() << "--" << reversePipe->getServicePointA();
                        first_iter = false;
                    }else{
                        pipeStream << " and " << reversePipe->getServicePointB() << "--" << reversePipe->getServicePointA();
                    }
                    continue;
                }
            }
            cout << "Error reading a pipe, be sure to type the correct codeA-codeB for each pipeline" << endl;
            cout << "<---Returning to menu function--->" << endl;
            cout << "\n";
            return;
        }
        if(pipe->isDirection()){
            graph->removeEdge(nodeMap[pipe->getServicePointA()]->getInfo(),nodeMap[pipe->getServicePointB()]->getInfo());
        }else{
            graph->removeEdge(nodeMap[pipe->getServicePointA()]->getInfo(),nodeMap[pipe->getServicePointB()]->getInfo());
            graph->removeEdge(nodeMap[pipe->getServicePointB()]->getInfo(),nodeMap[pipe->getServicePointA()]->getInfo());
        }
        if(first_iter){
            pipeStream << "/* Pipe rupture(s) from " << pipe->getServicePointA() << "--" << pipe->getServicePointB();
            first_iter = false;
        }else{
            pipeStream << " and " << pipe->getServicePointA() << "--" << pipe->getServicePointB();
        }
    }
    pipeStream << " */" <<endl;
    resetFlow(graph);
    edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
    for(auto city:cities){
        string code = city.second->getInfo()->getCode();
        City* pcity = (City*)nodeMap[code]->getInfo();
        int flow = 0;
        int oldFlow = 0;
        for(auto e:nodeMap[code]->getIncoming()){
            flow+=e->getFlow();
        }
        for(auto e:nodesKarpG[code]->getIncoming()){
            oldFlow+=e->getFlow();
        }
        if(flow < oldFlow){
            stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << oldFlow - flow << endl;
        }
    }
    cout << pipeStream.str();
    if(stream.str().empty()){
        cout << "There is no city affected by this/these pipe(s)" << endl;
        cout << "\n";
    }else{
        cout << stream.str();
        cout << "\n";
    }
    delete graph;
}


void Data::cleanUp() {
    for(auto tuple:pipes){
        delete tuple.second;
    }
    delete waterG;
    delete edmondskarpG;
}