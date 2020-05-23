cmake_minimum_required(VERSION 2.8)
project(SingSyn)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/libSinsy)

include_directories(lib/libSinsy/)
add_executable(SingSyn bin/SingSyn.cpp)

target_link_libraries(SingSyn libSingSyn)