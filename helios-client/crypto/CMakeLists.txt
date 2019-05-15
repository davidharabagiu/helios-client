cmake_minimum_required(VERSION 3.1)

project(crypto)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/aes128blockwork.h"
    "src/aes128blockwork.cpp"
    "inc/aes128blockencryptionwork.h"
    "src/aes128blockencryptionwork.cpp"
    "inc/aes128blockdecryptionwork.h"
    "src/aes128blockdecryptionwork.cpp"
    "inc/aes128defs.h"
    "inc/aes128tables.h"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
