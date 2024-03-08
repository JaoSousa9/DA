//
// Created by joaosousa on 28-02-2024.
//

#include "station.h"

int station::getId() const {
    return id;
}

void station::setId(int id) {
    station::id = id;
}

const string &station::getCode() const {
    return code;
}

void station::setCode(const string &code) {
    station::code = code;
}

station::station(int id, const string &code) : id(id), code(code) {}
