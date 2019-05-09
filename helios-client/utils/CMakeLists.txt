cmake_minimum_required(VERSION 3.1)

project(utils)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/paths.h"
    "inc/single.h"
    "inc/dependencyinjector.h"
    "inc/dependencyinjector.tpp"
    "src/dependencyinjector.cpp"
    "inc/typeconversions.h"
    "inc/executor.h"
    "inc/executor.tpp"
    "src/executor.cpp"
    "inc/typeutils.h"
    "inc/observable.h"
    "inc/timer.h"
    "src/timer.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
)
