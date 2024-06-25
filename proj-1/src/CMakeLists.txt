cmake_minimum_required(VERSION 3.26)
project(ProjetoDA1)

set(CMAKE_CXX_STANDARD 14)
#[===[
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
]===]
include_directories(.)
include_directories(data_structures)

add_executable(ProjetoDA1
        data_structures/Graph.h
        data_structures/MutablePriorityQueue.h
        data_structures/UFDS.cpp
        data_structures/UFDS.h
        City.cpp
        City.h
        Data.cpp
        Data.h
        main.cpp
        Menu.cpp
        Menu.h
        NodeData.cpp
        NodeData.h
        Pipe.cpp
        Pipe.h
        Reservoir.cpp
        Reservoir.h
        Station.cpp
        Station.h)