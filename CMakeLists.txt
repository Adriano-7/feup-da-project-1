cmake_minimum_required(VERSION 3.23)
project(Project_DA)

set(CMAKE_CXX_STANDARD 14)

include_directories(Data)
include_directories(src)

add_executable(Project_DA
        Data/network.csv
        Data/stations.csv
        src/station.cpp
        src/station.h
        data_structures/Graph.h
        data_structures/Graph.cpp
        data_structures/Heap.h
        data_structures/Heap.cpp
        data_structures/MutablePriorityQueue.h
        data_structures/MutablePriorityQueue.cpp
        data_structures/UFDS.cpp
        data_structures/UFDS.h
        data_structures/VertexEdge.cpp
        data_structures/VertexEdge.h)
