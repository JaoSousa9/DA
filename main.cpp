#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "data_structures/Graph.h"
#include "model/city.h"
#include "model/pipe.h"
#include "model/waterReservoir.h"
#include "model/station.h"
#include "controller/graphBuilder.h"
#include "algorithms/algo.cpp"

int main() {

    string path_cities = "/home/joaos/Documents/FEUP/DA/Project1LargeDataSet/Cities.csv";
    string path_reservoir = "/home/joaos/Documents/FEUP/DA/Project1LargeDataSet/Reservoir.csv";
    string path_stations = "/home/joaos/Documents/FEUP/DA/Project1LargeDataSet/Stations.csv";
    string path_pipes = "/home/joaos/Documents/FEUP/DA/Project1LargeDataSet/Pipes.csv";

    graphBuilder g;
    vector<string> cities = g.createNodes(path_cities);
    vector<string> reservoir = g.createNodes(path_reservoir);
    vector<string> stations = g.createNodes(path_stations);
    vector<pipe> pipes = g.createEdges(path_pipes);

    Graph<string> graph = g.getPortugalGraph();

    map<string, city> cityMap = g.buildCityMap(cities);
    map<string, station> stationMap = g.buildStationMap(stations);
    map<string, waterReservoir> waterReservoirMap = g.buildWaterReservoirMap(reservoir);

    string SUPER_SOURCE = "S";

    graph.addVertex(SUPER_SOURCE);

    for(string r : reservoir) {
        graph.addEdge(SUPER_SOURCE, r, INF);
    }

    double maxFLow = edmondsKarp(&graph, SUPER_SOURCE, "C_17");

    cout << " " << maxFLow;

    return 0;
}
