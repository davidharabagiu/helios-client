cmake_minimum_required(VERSION 3.1)

project(qtmodels)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/qtmodels.h"
    "src/qtmodels.cpp"
    "inc/qheliosfile.h"
    "src/qheliosfile.cpp"
    "inc/qfiletransfer.h"
    "src/qfiletransfer.cpp"
    "inc/qfilelisting.h"
    "src/qfilelisting.cpp"
    "inc/qtransferslist.h"
    "src/qtransferslist.cpp"
    "inc/qnotification.h"
    "src/qnotification.cpp"
    "inc/qusersession.h"
    "src/qusersession.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE $<TARGET_PROPERTY:models,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    models
    utils
)
