cmake_minimum_required(VERSION 3.10)
project(FlowShopBnb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(FlowShopBnb ${SOURCES})
