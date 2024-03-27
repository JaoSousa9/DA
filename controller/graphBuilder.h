//
// Created by joaosousa on 06-03-2024.
//

#ifndef PROJDA_GRAPHBUILDER_H
#define PROJDA_GRAPHBUILDER_H

#include <iostream>
#include <map>
#include "../data_structures/Graph.h"
#include "../model/city.h"
#include "../model/pipe.h"
#include "../model/station.h"
#include "../model/waterReservoir.h"

using namespace std;

class graphBuilder {
private:
    Graph<string> portugalGraph;
public:
    Graph<string> getPortugalGraph();
    void setPortugalGraph(const Graph<string> &portugalGraph);

    vector<pipe> createEdges(Graph<string>& portugalGraph);
    map<string, city> buildCityMap(Graph<string>& portugalGraph);
    map<string, station> buildStationMap(Graph<string>& portugalGraph);
    map<string, waterReservoir> buildWaterReservoirMap(Graph<string>& portugalGraph);
};


#endif //PROJDA_GRAPHBUILDER_H
