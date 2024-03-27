#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "controller/graphBuilder.h"
#include "algorithms/algo.cpp"

int main() {

    graphBuilder g = graphBuilder();
    Graph<string> portugalGraph = Graph<string>();

    map<string, city> cityMap = g.buildCityMap(portugalGraph);
    map<string, station> stationMap = g.buildStationMap(portugalGraph);
    map<string, waterReservoir> waterReservoirMap = g.buildWaterReservoirMap(portugalGraph);
    vector<pipe> pipes = g.createEdges(portugalGraph);

    string SUPER_SOURCE = "S";

    portugalGraph.addVertex(SUPER_SOURCE);

    for(auto r : waterReservoirMap) {
        portugalGraph.addEdge(SUPER_SOURCE, r.first, INF);
    }

    for(auto v : cityMap){

        double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first);

        cout << "This is the maximum amount of water that can reach " << cityMap.at(v.first).getName() << " " << maxFLow << endl;

        cout << "The annount " << cityMap.at(v.first).getDemand() << " is demanded." << endl;
    } 


    return 0;
}
