cmake_minimum_required(VERSION 3.23)
project(Project_DA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(data_structures)
include_directories(headers)
include_directories(src)

add_executable(Project_DA src/main.cpp src/Station.cpp headers/Station.h src/Database.cpp headers/Database.h src/Menu.cpp headers/Menu.h data_structures/Graph.cpp data_structures/Graph.h data_structures/NodeEdge.cpp data_structures/NodeEdge.h)