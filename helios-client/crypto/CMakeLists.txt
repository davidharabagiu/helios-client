cmake_minimum_required(VERSION 3.1)

project(crypto)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/private/aes/aescipher.h"
    "src/aes/aescipher.cpp"
    "inc/private/aes/commondefs.h"
    "inc/private/aes/tables.h"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    utils
)
