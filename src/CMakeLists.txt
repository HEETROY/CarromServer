cmake_minimum_required(VERSION 3.5)

project(CarromServer
        VERSION 0.0.1
        DESCRIPTION "My First Raw Socket C++ Server Build Using CMake For the first time"
        LANGUAGES CXX)

add_executable(server main.cpp network/server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# What this does
# CMake automatically adds the correct flags:
# Linux → -pthread
# Some systems → -lpthread
# Windows → nothing extra
# So you stay portable.
# If you don't add it
# When linking you may see errors like: undefined reference to pthread_create
# because std::thread internally calls pthread functions.
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)