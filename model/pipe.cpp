//
// Created by joaosousa on 28-02-2024.
//

#include "pipe.h"

const string &pipe::getSource() const {
    return source;
}

void pipe::setSource(const string &source) {
    pipe::source = source;
}

const string &pipe::getDest() const {
    return dest;
}

void pipe::setDest(const string &dest) {
    pipe::dest = dest;
}

int pipe::getCapacity() const {
    return capacity;
}

void pipe::setCapacity(int capacity) {
    pipe::capacity = capacity;
}

bool pipe::isDirection() const {
    return direction;
}

void pipe::setDirection(bool direction) {
    pipe::direction = direction;
}

pipe::pipe(const string &source, const string &dest, int capacity, bool direction) : source(source), dest(dest),
                                                                                     capacity(capacity),
                                                                                     direction(direction) {}
