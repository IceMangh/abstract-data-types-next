cmake_minimum_required(VERSION 4.0)
project(atd_next)

set(CMAKE_CXX_STANDARD 20)

add_executable(atd_next main.cpp Tests.cpp
        Hanoi.h
        PriorityQueue.h)
