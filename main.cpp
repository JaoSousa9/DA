#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "controller/graphBuilder.h"
#include "algorithms/algo.cpp"


int main() {

    cout << "+-------------------------------+" << endl;
    cout << "|                               |" << endl;
    cout << "|        Welcome to the         |" << endl;
    cout << "| Portugal Water Network Project|" << endl;
    cout << "|                               |" << endl;
    cout << "|                               |" << endl;
    cout << "+-------------------------------+" << endl;

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

    bool quit = false;

    while(!quit){

        cout << "Choose the desired task:" << endl;
        cout << "Maximum FLow of each City (1)" << endl;
        cout << "Cities in debt            (2)" << endl;
        cout << "Remove a reservoir        (4)" << endl;
        cout << "Remove a pipe             (5)" << endl;
        cout << "T3.3 (6)" << endl;
        cout << "QUIT (7)"  << endl;
        cout << "Choice: ";

        int task_choice;
        cin >> task_choice;
        
        cout << endl;
        cout << endl;

        string code;

        if(task_choice == 7){
            quit = true;
            break;
        }

        if(task_choice == 1){

            cout << "Choose a City:" << endl;
            for(auto v : cityMap){
                cout << cityMap.at(v.first).getName() << " " << cityMap.at(v.first).getCode() << endl;
            }
            cout << "ALL" << endl;

            cout << endl;
            cout << endl;

            cin >> code;

            if(code == "ALL"){
                for(auto v : cityMap){

                    double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                    double demand = cityMap.at(v.first).getDemand();

                    cout << cityMap.at(v.first).getName() + " has a maximum flow of " << maxFLow << " " << cityMap.at(v.first).getCode() << endl;
                }
            }

            if(cityMap.count(code)){
                city c = cityMap.at(code);

                double maxFlow = edmondsKarp(&portugalGraph, SUPER_SOURCE, c.getCode(), &waterReservoirMap, &cityMap);

                cout << "The city " << c.getName() << " gets " << maxFlow << "maxFLow" << endl;
            }else{
                cout << "Invalid City " << endl;
            }

            cout << endl;
            cout << endl;
            
            
        }if(task_choice == 2){
            for(auto v : cityMap){

                double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                double demand = cityMap.at(v.first).getDemand();

                if(demand > maxFLow){
                    cout << cityMap.at(v.first).getName() + " is in debt " << demand-maxFLow << " " << cityMap.at(v.first).getCode() << endl;
                } 
            }
            
            cout << endl;
            cout << endl;
        }if(task_choice == 4){
            //Remove a reservoir  -> find the adjacent edges to it -> Apply Edmonds Karp

            cout << endl;

            string code;

            cout << "Choose the water reservoir you desire to remove " << endl; 
            for(auto r : waterReservoirMap){
                cout << "    " << waterReservoirMap.at(r.first).getCode() << endl; 
            }

            cin >> code;

            //Print cities with deficit before
            for(auto v : cityMap){

                double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                double demand = cityMap.at(v.first).getDemand();

                if(demand > maxFLow){
                    cout << cityMap.at(v.first).getName() + " is in debt " << demand-maxFLow << " " << cityMap.at(v.first).getCode() << endl;
                } 
            }
            
            cout << endl;
            cout << endl;

            if(waterReservoirMap.count(code)){
                std::vector<Edge<string> * > adjacents = portugalGraph.findVertex(code)->getAdj();

                //Save edges wich reservoir was connected
                std::map<string , double> savedEdges;
                for(auto repair : adjacents){
                        savedEdges.insert(pair<string, double>(repair->getDest()->getInfo(), repair->getWeight()));
                }


                portugalGraph.removeEdge(SUPER_SOURCE, code);
                portugalGraph.removeVertex(code);

                //Debt with the reservoir removed
                for(auto v : cityMap){

                    double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                    double demand = cityMap.at(v.first).getDemand();

                    if(demand > maxFLow){
                        cout << cityMap.at(v.first).getName() + " is in debt " << demand-maxFLow << " " << cityMap.at(v.first).getCode() << endl;
                    } 
                }
                
                
                cout << endl;
                cout << endl;

                //Reconstruct the graph
                portugalGraph.addVertex(code);   
                for(auto repair : savedEdges){
                    portugalGraph.addEdge(code, repair.first , repair.second);
                }
                portugalGraph.addEdge(SUPER_SOURCE, code, INF);

            }
        }
    }



    return 0;
}
