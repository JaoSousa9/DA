//
// Created by joaosousa on 28-02-2024.
//

#ifndef PROJDA_CITY_H
#define PROJDA_CITY_H

#include <string>

using namespace std;

class city {

private:
    string name;
    int id;
    string code;
    int demand;
    int population;


public:
    city(string name, int id, string code, int demand, int population);

    const string &getName() const;

    void setName(string name);

    int getId();

    void setId(int id);

    const string &getCode() const;

    void setCode(string code);

    int getDemand();

    void setDemand(int demand);

    int getPopulation();

    void setPopulation(int population);

};


#endif //PROJDA_CITY_H
