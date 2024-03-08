//
// Created by joaosousa on 28-02-2024.
//

#include "waterReservoir.h"

const string &waterReservoir::getReservoir() const {
    return reservoir;
}

void waterReservoir::setReservoir(const string &reservoir) {
    waterReservoir::reservoir = reservoir;
}

const string &waterReservoir::getMunicipality() const {
    return municipality;
}

void waterReservoir::setMunicipality(const string &municipality) {
    waterReservoir::municipality = municipality;
}

int waterReservoir::getId() const {
    return id;
}

void waterReservoir::setId(int id) {
    waterReservoir::id = id;
}

const string &waterReservoir::getCode() const {
    return code;
}

void waterReservoir::setCode(const string &code) {
    waterReservoir::code = code;
}

int waterReservoir::getMaxCapacity() const {
    return maxCapacity;
}

void waterReservoir::setMaxCapacity(int maxCapacity) {
    waterReservoir::maxCapacity = maxCapacity;
}

waterReservoir::waterReservoir(const string &reservoir, const string &municipality, int id, const string &code,
                               int maxCapacity) : reservoir(reservoir), municipality(municipality), id(id), code(code),
                                                  maxCapacity(maxCapacity) {}
