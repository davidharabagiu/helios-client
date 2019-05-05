cmake_minimum_required(VERSION 3.1)

project(services)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/userservicelistener.h"
    "inc/public/serviceinterface.h"
    "inc/public/userservice.h"
    "inc/private/userserviceimpl.h"
    "src/userserviceimpl.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:settings,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:apicalls,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:models,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    utils
    settings
    apicalls
    models
)
