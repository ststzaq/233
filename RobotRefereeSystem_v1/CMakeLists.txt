cmake_minimum_required(VERSION 3.16)
project(RobotRefereeSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main
    main.cpp
    robot.cpp
    referee.cpp
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE -Wall -Wextra)
endif()

if(MSVC)
    target_compile_options(main PRIVATE /utf-8)
endif()
