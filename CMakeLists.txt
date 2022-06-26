cmake_minimum_required(VERSION 3.22)
project(ProjectEnigma)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProjectEnigma main.cpp)
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(ProjectEnigma sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
