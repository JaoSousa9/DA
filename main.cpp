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
            int line = 0;
            for(auto v : cityMap){
                cout << "(" + v.second.getName() << " " << v.second.getCode() + ")" << " ";
                if(line == 5){
                    line=0;
                    cout << endl;
                }
                line++;
            }

            cout << "ALL" << endl;

            cout << endl;
            cout << endl;

            cin >> code;

            if(code == "ALL"){
                for(auto v : cityMap){

                    double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                    double demand = v.second.getDemand();

                    cout << v.second.getName() + " has a maximum flow of " << maxFLow << " " << v.second.getCode() << endl;
                }
            }else if(cityMap.count(code)){
                city c = cityMap.at(code);

                double maxFlow = edmondsKarp(&portugalGraph, SUPER_SOURCE, c.getCode(), &waterReservoirMap, &cityMap);

                cout << "The city " << c.getName() << " gets " << maxFlow << " maxFLow" << endl;
            }else{
                cout << "Invalid City " << endl;
            }

            cout << endl;
            cout << endl;
            
            
        }if(task_choice == 2){
            for(auto v : cityMap){

                double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                double demand = v.second.getDemand();

                if(demand > maxFLow){
                    cout << v.second.getName() + " is in debt " << demand-maxFLow << " " << v.second.getCode() << endl;
                } 
            }
            
            cout << endl;
            cout << endl;
        }if(task_choice == 4){
            //Remove a reservoir  -> find the adjacent edges to it -> Apply Edmonds Karp

            cout << endl;

            string code;

            cout << "Choose the water reservoir you desire to remove " << endl; 

            int lines = 0;

            for(auto r : waterReservoirMap){
                cout << "  " << r.first;
                if(lines == 5){
                    lines = 0;
                    cout << endl;
                } 
                lines++;
            }
            cout << endl;

            cin >> code;

            std::map<string , double> deficitBefore;
            std::map<string , double> deficitAfter;

            //Print cities with deficit before
            for(auto v : cityMap){

                double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                double demand = v.second.getDemand();

                if(demand > maxFLow){
                    deficitBefore.insert(pair<string, double>(v.second.getName(),demand-maxFLow));
                    cout << v.second.getName() + " is in debt " << demand-maxFLow << " " << v.second.getCode() << endl;
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

                //Debt after reservoir removed
                for(auto v : cityMap){

                    double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                    double demand = v.second.getDemand();

                    if(demand > maxFLow){
                        deficitAfter.insert(pair<string, double>(v.second.getName(),demand-maxFLow));
                    } 
                }

                for(auto after : deficitAfter){
                    bool newCity = true;
                    for(auto before: deficitBefore){
                        if(after.first == before.first){
                            newCity = false;
                            if(after.second != before.second){
                                cout << "The city " + after.first + " got affected and the new deficit is " << after.second << endl;
                            }
                        }
                    }
                    if(newCity){
                        cout << "The city " + after.first + " got affected and the new deficit is " << after.second << endl;
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
        }if(task_choice == 5){
            //Remove a pumping station -> find the adjacent edges to it -> Apply Edmonds Karp
            
            std::map<string , double> deficitBefore;
            std::map<string , double> deficitAfter;
            cout << endl;

            string code;

            cout << "Choose the pumping station you desire to remove " << endl; 
            int lines = 0;
            for(auto r : stationMap){
                cout << "  " << r.first;
                if(lines == 20){
                    lines = 0;
                    cout << endl;
                } 
                lines++;
            }

            cin >> code;

            //Print cities with deficit before
            for(auto v : cityMap){

                double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                double demand = v.second.getDemand();

                if(demand > maxFLow){
                    deficitBefore.insert(pair<string, double>(v.second.getName(),demand-maxFLow));
                    cout << v.second.getName() + " is in debt " << demand-maxFLow << " " << v.second.getCode() << endl;
                } 
            }

            cout << endl;
            cout << endl;
            
             if(stationMap.count(code)){

                std::map<string , double> savedOutEdges;
                std::map<string , double> savedInEdges;
                std::vector<Edge<string> * > outgoing = portugalGraph.findVertex(code)->getAdj();
                std::vector<Edge<string> * > ingoing = portugalGraph.findVertex(code)->getIncoming();

                //Save the outgoing edges wich the pumping station was connected
                for(auto repair : outgoing){
                        savedOutEdges.insert(pair<string, double>(repair->getDest()->getInfo(), repair->getWeight()));
                }

                for(auto repair : ingoing){
                        savedInEdges.insert(pair<string, double>(repair->getOrig()->getInfo(), repair->getWeight()));
                }

                portugalGraph.removeVertex(code);

                //Debt after the pumping station is removed
                for(auto v : cityMap){

                    double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                    double demand = v.second.getDemand();

                    if(demand > maxFLow){
                        deficitAfter.insert(pair<string, double>(v.second.getName(),demand-maxFLow));
                    } 
                }
                

                for(auto after : deficitAfter){
                    bool newCity = true;
                    for(auto before: deficitBefore){
                        if(after.first == before.first){
                            newCity = false;
                            if(after.second != before.second){
                                cout << "The city " + after.first + " got affected and the new deficit is " << after.second << endl;
                            }
                        }
                    }
                    if(newCity){
                        cout << "The city " + after.first + " got affected and the new deficit is " << after.second << endl;
                    }
                }
                
                cout << endl;
                cout << endl;

                //Reconstruct the graph
                portugalGraph.addVertex(code);   
                for(auto repair : savedOutEdges){
                    portugalGraph.addEdge(code, repair.first , repair.second);
                }
                for(auto repair : savedInEdges){
                    portugalGraph.addEdge(repair.first, code, repair.second);
                }

            }
        }
    }



    return 0;
}
