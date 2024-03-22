//
// Created by joaosousa on 06-03-2024.
//

#include <sstream>
#include <algorithm>
#include <fstream>
#include "graphBuilder.h"

using namespace std;

vector<string> graphBuilder::createNodes(const string& path) {
    fstream fin;
    fin.open(path, ios::in);

    vector<string> rows;
    vector<string> codes;
    string CSV, temp;
    int codePos = 0;

    getline(fin, CSV , '\n');
    stringstream firstTuple(CSV);

    long numberOfTuples = count(CSV.begin(), CSV.end(),',');


    for(int i = 0; i <= numberOfTuples; i++){
         getline(firstTuple, temp, ',');

        if(temp == "Code"){
            codePos = i;
            break;
        }
        codePos=i;
    }

    //get all rows
    while (!fin.eof()) {
        getline(fin, CSV , '\n');
        rows.push_back(CSV);
    }

    for (string r: rows) {
        stringstream tuple(r);

        int i = 0;
        while(getline(tuple, r, ',')){
            if(codePos == i){
                codes.push_back(r);
                i++;
            }else{
                i++;
            }
        }
    }


    for(string c : codes){
        portugalGraph.addVertex(c);
    }
    return codes;
}

vector<pipe> graphBuilder::createEdges(const string &path) {
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
            portugalGraph.addEdge(source, dest, weight);
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


map<string, city> graphBuilder::buildCityMap(const vector<string>& cities) {

    string name;
    int id;
    string code;
    int demand;
    int population;
    map<string, city> citiesMap;

    for (string r: cities) {

        stringstream tuple(r);

        getline(tuple, name, ',');
        tuple >> id;
        tuple.ignore();
        getline(tuple, code, ',');
        tuple >> demand;
        tuple.ignore();
        tuple >> population;

        city c(name,id,code,demand,population);

        citiesMap.insert(pair<string, city>(code,c));
    }

    return citiesMap;
}


map<string, station> graphBuilder::buildStationMap(const vector<string>& stations) {

    int id;
    string code;
    map<string, station> stationsMap;

    for (string s: stations) {

        stringstream tuple(s);

        tuple >> id;
        tuple.ignore();
        getline(tuple, code, ',');

        station st(id,code);

        stationsMap.insert(pair<string, station>(code,st));
    }
    return stationsMap;
}

map<string, waterReservoir> graphBuilder::buildWaterReservoirMap(const vector<string>& reservoirs) {

    string reservoir;
    string municipality;
    int id;
    string code;
    int maxCapacity;
    map<string, waterReservoir> reservoirMap;

    for (string r: reservoirs) {

        stringstream tuple(r);

        getline(tuple, reservoir, ',');
        getline(tuple, municipality, ',');
        tuple >> id;
        tuple.ignore();
        getline(tuple, code, ',');
        tuple >> maxCapacity;

        waterReservoir wr(reservoir,municipality,id,code,maxCapacity);

        reservoirMap.insert(pair<string, waterReservoir>(code,wr));
    }

    return reservoirMap;
}
