//
// Created by joaosousa on 28-02-2024.
//

#include "city.h"

const string &city::getName() const{
    return name;
}

void city::setName(string city) {
    city::name = city;
}

int city::getId(){
    return id;
}

void city::setId(int id) {
    city::id = id;
}

const string &city::getCode() const{
    return code;
}

void city::setCode(string code) {
    city::code = code;
}

int city::getDemand(){
    return demand;
}

void city::setDemand(int demand) {
    city::demand = demand;
}

int city::getPopulation(){
    return population;
}

void city::setPopulation(int population) {
    city::population = population;
}

city::city(string name, int id, string code, int demand, int population) : name(name), id(id),
                                                                                         code(code), demand(demand),
                                                                                         population(population) {}
