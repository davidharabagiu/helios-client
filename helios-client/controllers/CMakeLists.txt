cmake_minimum_required(VERSION 3.1)

project(controllers)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/controllers.h"
    "src/controllers.cpp"
    "inc/public/qauthenticationcontroller.h"
    "src/qauthenticationcontroller.cpp"
    "inc/private/qauthenticationcontrollerimpl.h"
    "src/qauthenticationcontrollerimpl.cpp"
    "inc/public/qsettingscontroller.h"
    "src/qsettingscontroller.cpp"
    "inc/private/qsettingscontrollerimpl.h"
    "src/qsettingscontrollerimpl.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE $<TARGET_PROPERTY:models,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:qtmodels,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:services,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:settings,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:config,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    models
    qtmodels
    services
    settings
    config
)
