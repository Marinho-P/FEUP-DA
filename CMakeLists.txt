cmake_minimum_required(VERSION 3.27)
project(ProjetoDA2)

set(CMAKE_CXX_STANDARD 14)

include_directories(data_structures)

add_executable(ProjetoDA2
        Menu.cpp
        main.cpp
        Vertex.h
        Vertex.cpp
        Edge.h
        Graph.h
        Graph.cpp
        Edge.cpp)
