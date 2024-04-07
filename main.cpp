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
        cout << "Balancing the graph       (3)" << endl;
        cout << "Remove a reservoir        (4)" << endl;
        cout << "Remove a Pumping Station  (5)" << endl;
        cout << "Remove a Pipe             (6)" << endl;
        cout << "QUIT                      (7)" << endl;
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
            
            
        }else if(task_choice == 2){
            for(auto v : cityMap){

                double maxFLow = edmondsKarp(&portugalGraph, SUPER_SOURCE, v.first, &waterReservoirMap, &cityMap);
                double demand = v.second.getDemand();

                if(demand > maxFLow){
                    cout << v.second.getName() + " is in debt " << demand-maxFLow << " " << v.second.getCode() << endl;
                } 
            }
            
            cout << endl;
            cout << endl;
        }else if (task_choice == 3){

            balanceLoad(&portugalGraph, &waterReservoirMap, &cityMap);


        }else if(task_choice == 4){
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
        }else if(task_choice == 5){
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
        }else if(task_choice == 6){
            cout << endl;
            vector<pipe*> removed;
            map<string,double> oldFlows;

            // Calculate flows before removing any pipes
            for(auto &dsPair : cityMap) {
                const string cityCode = dsPair.first;
                double flow = edmondsKarp(&portugalGraph, SUPER_SOURCE, cityCode, &waterReservoirMap, &cityMap);
                oldFlows.insert(pair<string, double>(cityCode, flow));
            }

            int code;
            // Remove 1 or more pipes
            while (true) {
                cout << "Choose the pipe you desire to remove " << endl;
                cout << "    -1: end" << endl;
                map<int, pipe*> pipeMap;
                int idx = 0;
                int lines = 0;
                for (auto &r: pipes) {
                    if(lines == 5){
                        lines = 0;
                        cout << endl;
                    }
                    pipeMap.insert(pair<int, pipe*>(idx, &r));
                    cout << "    " << idx << ": (" << r.getSource() << " " << r.getDest() << ") ";
                    idx++, lines++;
                }
                cin >> code;
                if(code == -1)
                    break;

                if (code >= 0 && code < idx) {
                    pipe* p = pipeMap.at(code);
                    portugalGraph.removeEdge(p->getSource(), p->getDest());
                    cout << "Removed: (" << p->getSource() << "," << p->getDest() << ")" << endl;
                    if (!p->isDirection()) {
                        portugalGraph.removeEdge(p->getDest(), p->getSource());
                        cout << "Removed: (" << p->getDest() << "," << p->getSource() << ")" << endl;
                    }
                    removed.push_back(p);
                    pipes.erase(pipes.begin() + code);
                }else{
                    cout << "Select a valid number" << endl;
                }
            }
            if(code == -1 && removed.empty()){
                cout << "Nothing to do... No pipes were removed" << endl;
                break;
            }

            // Calculate flows after removing pipes
            cout << "City Code,Demand,Old Flow,New Flow" << endl;
            for(auto &dsPair : cityMap) {
                const string cityCode = dsPair.first;
                city ct = dsPair.second;

                string name = ct.getName();
                double demand = ct.getDemand();
                double oldFlow = oldFlows.at(cityCode);
                double newFlow = edmondsKarp(&portugalGraph, SUPER_SOURCE, cityCode, &waterReservoirMap, &cityMap);

                if (oldFlow == newFlow) continue;

                cout << "("   << cityCode  << ", "
                     << fixed << name      << ", "
                     << fixed << demand    << ", "
                     << fixed << oldFlow   << ", "
                     << fixed << newFlow   << ") "
                     << endl;
            }
            cout << endl << endl;


            // Restore pipes
            for(auto &i : removed){
                pipes.push_back(*i);
                if(!i->isDirection())
                    portugalGraph.addBidirectionalEdge(i->getSource(), i->getDest(), i->getCapacity());
                else
                    portugalGraph.addEdge(i->getSource(), i->getDest(), i->getCapacity());
            }
        }
    }



    return 0;
}
