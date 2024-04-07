#include "../data_structures/Graph.h"
#include "../model/waterReservoir.h"
#include "../model/city.h"
#include <iostream>
#include <map>

template <class T>
void testAndVisit(std::queue<Vertex<T> *> &q, Edge<T> *e, Vertex<T> *w, double residual)
{
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (!w->isVisited() && residual > 0)
    {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}
// Function to find an augmenting path using Breadth-First Search
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t)
{

    // Mark all vertices as not visited
    for (auto v : g->getVertexSet())
    {
        v->setVisited(false);
    }
    // Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<T> *> q;
    q.push(s);
    // BFS to find an augmenting path
    while (!q.empty() && !t->isVisited())
    {
        auto v = q.front();
        q.pop();
        // Process outgoing edges
        for (auto e : v->getAdj())
        {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        // Process incoming edges
        for (auto e : v->getIncoming())
        {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}

// Function to find the minimum residual capacity along the augmenting path
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t)
{
    double f = INF;

    // Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s;)
    {
        auto e = v->getPath();
        if (e->getDest() == v)
        {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else
        {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    // Return the minimum residual capacity
    return f;
}
// Function to augment flow along the augmenting path with the given flow value
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f, std::map<std::string, waterReservoir> *m)
{

    // Traverse the augmenting path and update the flow values accordingly to f
    for (auto v = t; v != s;)
    {
        if(v->getInfo()[0] == 'R'){
            int ammount = m->at(v->getInfo()).getMaxCapacity();
            m->at(v->getInfo()).setMaxCapacity(ammount - f);
        }
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v)
        {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else
        {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
template <class T>
double edmondsKarp(Graph<T> *g, std::string source, std::string target, std::map<std::string, waterReservoir> *m, std::map<std::string, city> *c)
{

    // Find source and target vertices in the graph
    Vertex<T> *s = g->findVertex(source);
    Vertex<T> *t = g->findVertex(target);

    double demand = c->at(t->getInfo()).getDemand();

    // Validate source and target vertices
    if (s == nullptr)
        throw std::logic_error("Invalid source vertex");
    if (t == nullptr)
        throw std::logic_error("Invalid target vertex");
    if (s == t)
        throw std::logic_error("Invalid input, source equals target vertex");

    double maxFlow = 0.0;
    // Initialize flow on all edges to 0
    for (auto v : g->getVertexSet())
    {
        for (auto e : v->getAdj())
        {
            e->setFlow(0);
        }
    }
    // While there is an augmenting path, augment the flow along the path
    while (findAugmentingPath(g, s, t))
    {
        // While there is paths from s to t
        double f = findMinResidualAlongPath(s, t);
        if(demand < maxFlow + f){
            f = demand;
            augmentFlowAlongPath(s, t, f, m);
            return demand;
        }else{
        // remove flow f from that path backwards
            augmentFlowAlongPath(s, t, f, m);
            maxFlow += f;
        }

    }

    return maxFlow;
}


/**
 * @brief Function to augment flow along the augmenting path with the given flow value modified for task 2.3. 
 * 
 * @tparam T 
 * @param s 
 * @param t 
 * @param f 
 * @param m 
 * @return double 
 */
template <class T>
double augmentFlowAlongPath_modified(Vertex<T>* s, Vertex<T>* t, double f, std::map<std::string, waterReservoir>* m) {
    double actualFlow = f; // Assume actual flow is equal to f initially

    // Traverse the augmenting path and update the flow values accordingly to f
    for (auto v = t; v != s;) {
        // Skip processing edges with infinite capacity or undirected edges
        auto e = v->getPath();
        if (e == nullptr || e->getWeight() == std::numeric_limits<double>::infinity()) {
            v = (e != nullptr && e->getOrig() == v) ? e->getDest() : e->getOrig();
            continue; // Skip this iteration
        }

        if (v->getInfo()[0] == 'R') {
            int amount = m->at(v->getInfo()).getMaxCapacity();
            if (amount < f) {
                actualFlow = amount; // Adjust actual flow based on reservoir capacity
            }
            m->at(v->getInfo()).setMaxCapacity(amount - actualFlow); // Update reservoir capacity
        }

        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + actualFlow);
            v = e->getOrig();
        } else {
            e->setFlow(flow - actualFlow);
            v = e->getDest();
        }
    }

    return actualFlow; // Return the actual flow applied along the path
}

template <class T>
double edmondsKarp_modified(Graph<T>* g, std::string source, std::string target,
                            std::map<std::string, waterReservoir>* m, std::map<std::string, city>* c) {
    // Find source and target vertices in the graph
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);

    // Validate source and target vertices
    if (s == nullptr) {
        std::cerr << "Error: Invalid source vertex" << std::endl;
        return 0.0; // Return an appropriate value or throw an exception as needed
    }
    if (t == nullptr) {
        std::cerr << "Error: Invalid target vertex" << std::endl;
        return 0.0; // Return an appropriate value or throw an exception as needed
    }
    if (s == t) {
        std::cerr << "Error: Source vertex equals target vertex" << std::endl;
        return 0.0; // Return an appropriate value or throw an exception as needed
    }

    double totalFlow = 0.0; // Initialize total flow to 0
    // Initialize flow on all edges to 0
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }

    

    // While there is an augmenting path, augment the flow along the path
    while (findAugmentingPath(g, s, t)) {
        double actualFlow = 0.0; // Initialize actual flow for this iteration

        // While there are paths from s to t
        double f = findMinResidualAlongPath(s, t);
        if (c->at(t->getInfo()).getDemand() < totalFlow + f) {
            f = c->at(t->getInfo()).getDemand() - totalFlow; // Adjust f to meet demand
            actualFlow = augmentFlowAlongPath_modified(s, t, f, m);
        } else {
            actualFlow = augmentFlowAlongPath_modified(s, t, f, m);
        }

        totalFlow += actualFlow; // Update total flow with actual flow applied
    }

    return totalFlow; // Return the total flow
}

static void balanceLoad(Graph<string>* g, map<string, waterReservoir>* waterReservoirMap, map<string, city>* cityMap) {
    // Compute initial load metrics (average, variance, max difference)
    double totalDifference = 0.0; // Initialize total difference


    g->removeVertex("S");
    // Calculate initial metrics
    int numPipes = 0; // Counter for the number of pipes
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e->getWeight() > 0) { // Consider only pipes with capacity (exclude super source)
                numPipes++; // Increment the number of pipes
                double capacity = e->getWeight();
                double flow = e->getFlow();
                double difference = capacity - flow; // Compute the difference
                totalDifference += difference; // Accumulate differences for computing average later

                // Adjust flow based on average difference
                double targetFlow = capacity - (totalDifference / numPipes); // Target flow based on average difference
                double newFlow = flow + (targetFlow - flow); // Adjust flow towards the target

                e->setFlow(newFlow); // Update flow on the pipe
            }
        }
    }

    

    double averageDifference = totalDifference / numPipes; // Calculate average difference

    cout << "Initial Load Metrics:" << endl;
    cout << "Total Difference: " << totalDifference << endl;
    cout << "Average Difference: " << averageDifference << endl;

    // Recalculate load metrics after balancing using the modified algorithm
    double newTotalDifference = 0.0; // Initialize new total difference
    int numPipesAfter = 0; // Counter for the number of pipes after balancing
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e->getWeight() > 0) { // Consider only pipes with capacity (exclude super source)
                numPipesAfter++; // Increment the number of pipes after balancing
                double capacity = e->getWeight();
                double flow = e->getFlow();
                double difference = capacity - flow; // Compute the difference
                newTotalDifference += difference; // Accumulate differences for computing average later

                // Adjust flow based on average difference using modified Edmonds-Karp
                double targetFlow = capacity - (newTotalDifference / numPipesAfter); // Target flow based on average difference
                double newFlow = flow + (targetFlow - flow); // Adjust flow towards the target

                e->setFlow(newFlow); // Update flow on the pipe
            }
        }
    }

    double newAverageDifference = newTotalDifference / numPipesAfter; // Calculate average difference after balancing


    string SUPER_SOURCE = "S";

    g->addVertex(SUPER_SOURCE);

    for (auto it = waterReservoirMap->begin(); it != waterReservoirMap->end(); ++it) {
          g->addEdge(SUPER_SOURCE, it->first, INF);
    }

    string SUPER_SYNK = "T";

    g->addVertex(SUPER_SYNK);

    for (auto it = cityMap->begin(); it != cityMap->end(); ++it) {
          g->addEdge(it->first, SUPER_SYNK, INF);
    }
    // Call the modified Edmonds-Karp algorithm to recalculate flow based on updated capacities
    double totalFlow = edmondsKarp_modified(g, "S", "T", waterReservoirMap, cityMap);


    cout << "Load Metrics After Balancing:" << endl;
    cout << "New Total Difference: " << newTotalDifference << endl;
    cout << "New Average Difference: " << newAverageDifference << endl;
}