cmake_minimum_required(VERSION 3.1)
project(settings)

SET(DEFAULT_SETTINGS_FILE "default_settings.json")

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/defaultsettingskeys.h"
    "inc/public/settingsmanager.h"
    "inc/private/settingsmanagerimpl.h"
    "src/settingsmanagerimpl.cpp"
    "inc/private/defaultsettingsprovider.h"
    "inc/private/defaultsettingsproviderimpl.h"
    "src/defaultsettingsproviderimpl.cpp"
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

add_custom_target(default_settings SOURCES ${DEFAULT_SETTINGS_FILE})

install(FILES ${DEFAULT_SETTINGS_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX})
