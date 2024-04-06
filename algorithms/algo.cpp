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