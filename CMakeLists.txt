cmake_minimum_required(VERSION 3.12)
project(zad2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

add_executable(zad2 netstore-server.c netstore-client.c)