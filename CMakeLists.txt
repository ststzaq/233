cmake_minimum_required(VERSION 3.15)

project(percept LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(percept
    main.cpp
    percept.cpp
)
