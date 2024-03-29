//
// Created by joaosousa on 06-03-2024.
//
#include <cstring>  
#include <sstream>
#include <algorithm>
#include <fstream>
#include "graphBuilder.h"

using namespace std;

vector<pipe> graphBuilder::createEdges(Graph<string>& portugalGraph) {
    string path = "Project1LargeDataSet/Pipes.csv";
    fstream fin;
    fin.open(path, ios::in);

    vector<string> rows;
    vector<pipe> pipes;
    string CSV;

    getline(fin, CSV , '\n');

    while (!fin.eof()) {
        getline(fin, CSV , '\n');
        rows.push_back(CSV);
    }

    for (string r: rows) {
        string source, dest;
        int weight, direction;
        stringstream tuple(r);

        getline(tuple, source, ',');
        getline(tuple, dest, ',');
        tuple >> weight;
        tuple.ignore();
        tuple >> direction;

        pipe p(source, dest, weight, direction);
        pipes.push_back(p);

        if(direction ==  0){
            portugalGraph.addBidirectionalEdge(source, dest, weight);
        }else{
            bool r = portugalGraph.addEdge(source, dest, weight);
        }
    }


    return pipes;

}

void graphBuilder::setPortugalGraph(const Graph<std::string> &portugalGraph) {
    graphBuilder::portugalGraph = portugalGraph;
}

Graph<string> graphBuilder::getPortugalGraph(){
    return portugalGraph;
}


map<string, city> graphBuilder::buildCityMap(Graph<string>& portugalGraph) {

    fstream fin;
    fin.open("Project1LargeDataSet/Cities.csv", ios::in);

    string name;
    int id;
    string code;
    int demand;
    int population;
    map<string, city> citiesMap;

    string CSV, temp;

    getline(fin, CSV , '\n');
    stringstream firstTuple(CSV);

    //get all rows
    while (!fin.eof()) {

        getline(fin, name, ',');
        fin >> id;
        fin.ignore();
        getline(fin, code, ',');
        fin >> demand;
        fin.ignore();
        fin >> population;
        getline(fin, temp);

        city c(name,id,code,demand,population);

        portugalGraph.addVertex(code);
        citiesMap.insert(pair<string, city>(code,c));
    }

    return citiesMap;
}


map<string, station> graphBuilder::buildStationMap(Graph<string>& portugalGraph) {

    fstream fin;
    fin.open("Project1LargeDataSet/Stations.csv", ios::in);

    map<string, station> stationsMap;

    string CSV;
    getline(fin, CSV , '\n');

    string line;
    while (getline(fin, line)) {


        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int id = stoi(token);
        getline(ss, token, ','); 

        token.erase(0, token.find_first_not_of(" \t\r\n"));
        string code = token.erase(token.find_last_not_of(" \t\r\n") + 1);
        station st(id, code);


        portugalGraph.addVertex(code);
        stationsMap.insert(pair<string, station>(code,st)); 

    }

    return stationsMap;
}

map<string, waterReservoir> graphBuilder::buildWaterReservoirMap(Graph<string>& portugalGraph) {

    fstream fin;
    fin.open("Project1LargeDataSet/Reservoir.csv", ios::in);

    string reservoir;
    string municipality;
    int id;
    string code;
    int maxCapacity;
    map<string, waterReservoir> reservoirMap;

    string CSV, temp;

    getline(fin, CSV , '\n');
    stringstream firstTuple(CSV);

    //get all rows
    while (!fin.eof()) {

        getline(fin, reservoir, ',');
        getline(fin, municipality, ',');
        fin >> id;
        fin.ignore();
        getline(fin, code, ',');
        fin >> maxCapacity;

        waterReservoir wr(reservoir,municipality,id,code,maxCapacity);

        portugalGraph.addVertex(code);
        reservoirMap.insert(pair<string, waterReservoir>(code,wr));
    }

    return reservoirMap;
}
