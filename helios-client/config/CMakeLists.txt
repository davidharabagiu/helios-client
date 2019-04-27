cmake_minimum_required(VERSION 3.1)
project(config)

SET(CONFIG_FILE "config.json")

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/configkeys.h"
    "inc/public/config.h"
    "inc/private/configimpl.h"
    "src/configimpl.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    utils
)

add_custom_target(configurations SOURCES ${CONFIG_FILE})

install(FILES ${CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX})
