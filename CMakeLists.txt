cmake_minimum_required(VERSION 3.10)

project(Project VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)

set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

find_package(SFML 2.6 COMPONENTS window system graphics audio network REQUIRED)
target_link_libraries(server PRIVATE sfml-window sfml-system sfml-graphics sfml-audio sfml-network)
target_link_libraries(client PRIVATE sfml-window sfml-system sfml-graphics sfml-audio sfml-network)
