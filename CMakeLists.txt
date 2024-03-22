cmake_minimum_required(VERSION 3.27)
project(projDA)

set(CMAKE_CXX_STANDARD 17)

add_executable(projDA main.cpp
        model/waterReservoir.cpp
        model/waterReservoir.h
        model/pipe.cpp
        model/pipe.h
        model/city.cpp
        model/city.h
        model/station.cpp
        model/station.h
        controller/graphBuilder.cpp
        controller/graphBuilder.h
        data_structures/UFDS.cpp
        algorithms/algo.cpp)
