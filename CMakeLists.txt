cmake_minimum_required(VERSION 4.1.0)


project(main CXX)

set(LIB_RBT ./RBt/robot_type.cpp)
add_library(rbt SHARED ${LIB_RBT})

set(LIB_JUD ./Judge/jud.cpp)
add_library(jud SHARED ${LIB_JUD})

target_link_libraries(jud PUBLIC rbt) 

target_include_directories(rbt PUBLIC ${PROJECT_SOURCE_DIR}/RBt)
target_include_directories(jud PUBLIC ${PROJECT_SOURCE_DIR}/Judge)

add_executable(main main.cpp)

target_link_libraries(main rbt)
target_link_libraries(main jud)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)