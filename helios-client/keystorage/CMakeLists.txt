cmake_minimum_required(VERSION 3.1)

project(keystorage)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/keymanager.h"
    "inc/private/keymanagerimpl.h"
    "src/keymanagerimpl.cpp"
    "inc/private/key.h"
    "src/key.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    utils
)
