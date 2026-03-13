cmake_minimum_required(VERSION 3.5)
project(CarromServer
        VERSION 0.0.1
        DESCRIPTION "My First Raw Socket C++ Server Build Using CMake For the first time"
        LANGUAGES CXX)

add_executable(server main.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)