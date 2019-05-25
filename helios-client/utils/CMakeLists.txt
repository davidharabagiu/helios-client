cmake_minimum_required(VERSION 3.1)

project(utils)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/paths.h"
    "inc/single.h"
    "inc/dependencyinjector.h"
    "inc/dependencyinjector.tpp"
    "src/dependencyinjector.cpp"
    "inc/typeconversions.h"
    "inc/typeutils.h"
    "inc/observable.h"
    "inc/pathutils.h"
    "src/pathutils.cpp"
    "inc/bigintegeralgorithmsextended.h"
    "src/bigintegeralgorithmsextended.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE $<TARGET_PROPERTY:async,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:crypto,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE $ENV{BIGINT_DIR}/include
)

find_library(BIGINT_LIB bigint $ENV{BIGINT_DIR}/lib)

target_link_libraries(${PROJECT_NAME}
    ${BIGINT_LIB}
    async
    crypto
)
