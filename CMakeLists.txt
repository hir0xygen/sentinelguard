cmake_minimum_required(VERSION 3.28)
project(sentinelguard)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-lboost_program_options")

add_executable(sentinelguard main.cpp
        protections.cpp
        protections.hpp)
