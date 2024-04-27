cmake_minimum_required(VERSION 3.27)
project(ProjetoDA2)

set(CMAKE_CXX_STANDARD 14)

include_directories(data_structures)

add_executable(ProjetoDA2
        data_structures/Graph.h
        data_structures/MutablePriorityQueue.h
        data_structures/UFDS.cpp
        data_structures/UFDS.h
        Menu.cpp
        Data.cpp
        main.cpp
        Coordinate.cpp)
