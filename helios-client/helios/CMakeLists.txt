cmake_minimum_required(VERSION 3.1)

project(helios)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/rpath_config.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_executable(${PROJECT_NAME}
    "src/main.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:qtmodels,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:controllers,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:services,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:apicalls,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:settings,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:http,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:config,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:crypto,INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    utils
    qtmodels
    controllers
    services
    apicalls
    settings
    http
    config
    crypto
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
