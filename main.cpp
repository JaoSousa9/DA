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

int main() {

    string path_cities = "/home/joaosousa/Documents/DA/projDA/Project1LargeDataSet/Cities.csv";
    string path_reservoir = "/home/joaosousa/Documents/DA/projDA/Project1LargeDataSet/Reservoir.csv";
    string path_stations = "/home/joaosousa/Documents/DA/projDA/Project1LargeDataSet/Stations.csv";
    string path_pipes = "/home/joaosousa/Documents/DA/projDA/Project1LargeDataSet/Pipes.csv";

    graphBuilder g;
    vector<string> cities = g.createNodes(path_cities);
    vector<string> reservoir = g.createNodes(path_reservoir);
    vector<string> stations = g.createNodes(path_stations);
    vector<pipe> pipes = g.createEdges(path_pipes);

    map<string, city> cityMap = g.buildCityMap(cities);
    map<string, station> stationMap = g.buildStationMap(stations);
    map<string, waterReservoir> waterReservoirMap = g.buildWaterReservoirMap(reservoir);


    /*for (const auto& pair : cityMap) {
        cout << "Value for key " <<  pair.first << ": " << pair.second.getName() << endl;

    for (const auto& pair : stationMap) {
        cout << pair.first << endl;
    }
    for (const auto& pair : waterReservoirMap) {
        cout << "Value for key " <<  pair.first << ": " << pair.second.getMunicipality() << "reservoir " << pair.second.getReservoir()<< endl;
    }*/
    return 0;
}
