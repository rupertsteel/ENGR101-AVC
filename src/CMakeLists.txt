
cmake_minimum_required(VERSION 2.8)

add_library(libe101 STATIC IMPORTED)
set_target_properties(libe101 PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libE101.so)

add_executable(robot main.cpp open-gate.cpp movement.cpp)

target_link_libraries(robot libe101)


