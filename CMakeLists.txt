cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-ggdb -Wall -Wextra -O3 -march=native -flto=auto")

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
 set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)
endif()

set(HEADERS src/json.hpp
            src/market.h
            src/computer.h
            src/solution.h
            src/instance.h
            src/algorithms.h
            src/Genetic.h)

set(SOURCES src/main.cpp)

set(PROJECT_NAME "sem")

project(${PROJECT_NAME} LANGUAGES CXX)
add_EXECUTABLE(${PROJECT_NAME} ${HEADERS} ${SOURCES})
