cmake_minimum_required(VERSION 3.16)

project(RefereeSystem LANGUAGES CXX)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=GBK)
set(SRC_LIST src/main.cpp src/Refree_System.cpp )
MESSAGE(STATUS "This is BINARY dir " ${RefereeSystem_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${RefereeSystem_SOURCE_DIR})
ADD_EXECUTABLE(RefereeSystem ${SRC_LIST})