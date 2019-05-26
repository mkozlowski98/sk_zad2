cmake_minimum_required(VERSION 3.12)
project(zad2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

add_executable(zad2 netstore-server.cpp structures.cpp structures.cpp netstore-client.cpp netstore-client.h)