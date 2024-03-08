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

    vector<string> createNodes(const string& path);
    vector<pipe> createEdges(const string& path);
    map<string, city> buildCityMap(const vector<string>& cities);
    map<string, station> buildStationMap(const vector<string>& station);
    map<string, waterReservoir> buildWaterReservoirMap(const vector<string>& reservoir);
};


#endif //PROJDA_GRAPHBUILDER_H
