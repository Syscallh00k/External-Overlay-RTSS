cmake_minimum_required(VERSION 4.1)
project(RTSSOverlay)

set(CMAKE_CXX_STANDARD 20)

add_executable(RTSSOverlay main.cpp
                           RTSS/rtss.cpp
                           utils/log/log.cpp)
