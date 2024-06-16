cmake_minimum_required(VERSION 3.10)
project(sentinelguard)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finline-functions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -floop-parallelize-all")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize")

add_executable(sentinelguard main.cpp
        protections.cpp
        protections.hpp)