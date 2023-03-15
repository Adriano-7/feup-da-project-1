cmake_minimum_required(VERSION 3.23)
project(Project_DA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(data_structures)
include_directories(headers)
include_directories(src)

add_executable(Project_DA src/main.cpp headers/station.h src/station.cpp data_structures/Graph.h data_structures/Graph.cpp data_structures/Heap.h data_structures/Heap.cpp data_structures/MutablePriorityQueue.h data_structures/UFDS.h data_structures/UFDS.cpp data_structures/VertexEdge.h data_structures/VertexEdge.cpp)