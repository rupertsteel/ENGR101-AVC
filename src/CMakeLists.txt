
cmake_minimum_required(VERSION 2.8)

add_library(libe101 STATIC IMPORTED)

# This is horrable, but we are using cmake 2.x not cmake 3.x
set_target_properties(libe101 PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libE101.so)

add_executable(robot main.cpp open-gate.cpp movement.cpp analysePicture.cpp IRsensor.cpp)

set_target_properties(robot PROPERTIES COMPILE_FLAGS --std=c++0x)

target_link_libraries(robot libe101)


