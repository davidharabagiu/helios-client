cmake_minimum_required(VERSION 3.1)

project(models)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/useraccount.h"
    "src/useraccount.cpp"
    "inc/usersession.h"
    "src/usersession.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
