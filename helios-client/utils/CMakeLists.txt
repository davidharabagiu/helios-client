cmake_minimum_required(VERSION 3.1)

project(utils)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/single.h"
    "inc/dependencyinjector.h"
    "inc/dependencyinjector.tpp"
    "inc/misc.h"
    "src/dependencyinjector.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
