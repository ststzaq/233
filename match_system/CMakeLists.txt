cmake_minimum_required(VERSION 3.10)
project(match_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(match_system main.cpp robot.cpp referee.cpp)
