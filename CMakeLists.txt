cmake_minimum_required(VERSION 3.20)
project(water_cube_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(water_cube_sim
    src/main.cpp
)
