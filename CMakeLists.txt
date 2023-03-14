cmake_minimum_required(VERSION 3.23)
project(Project_DA)

set(CMAKE_CXX_STANDARD 14)

include_directories(Data)
include_directories(src)

add_executable(Project_DA
        Data/network.csv
        Data/network.png
        Data/stations.csv
        src/station.cpp
        src/station.h)
