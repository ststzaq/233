cmake_minimum_required(VERSION 3.10)

project(RefereeSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(referee_exe
        main.cpp
        referee.cpp
)