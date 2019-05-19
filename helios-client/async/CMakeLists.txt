cmake_minimum_required(VERSION 3.1)

project(async)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/autoresetevent.h"
    "src/autoresetevent.cpp"
    "inc/customactionevent.h"
    "src/customactionevent.cpp"
    "inc/executor.h"
    "inc/executor.tpp"
    "src/executor.cpp"
    "inc/manualresetevent.h"
    "src/manualresetevent.cpp"
    "inc/timer.h"
    "src/timer.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    utils
)
