# Kudos https://github.com/ttroy50/cmake-examples/blob/master/01-basic/B-hello-headers/CMakeLists.txt
cmake_minimum_required(VERSION 4.0)

# Set the project name
project (andromeda)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -lm")

file (GLOB SOURCES "src/*.cpp")

add_executable(andromeda ${SOURCES})

target_include_directories(andromeda
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
