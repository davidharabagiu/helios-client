cmake_minimum_required(VERSION 3.1)

project(crypto)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/private/aes/work.h"
    "src/aes/work.cpp"
    "inc/private/aes/encryptionwork.h"
    "src/aes/encryptionwork.cpp"
    "inc/private/aes/decryptionwork.h"
    "src/aes/decryptionwork.cpp"
    "inc/private/aes/commondefs.h"
    "inc/private/aes/tables.h"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
)
