cmake_minimum_required(VERSION 4.2)
project(virtualFiles)

set(CMAKE_CXX_STANDARD 20)

add_executable(virtualFiles main.cpp
        Node.cpp
        Node.h)
