cmake_minimum_required(VERSION 3.1)

project(unittests)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/rpath_config.cmake REQUIRED)

add_executable(${PROJECT_NAME}
    "src/main.cpp"
    "src/dependencyinjector_tests.cpp"
    "src/typeconversions_tests.cpp"
    "src/formdatarequest_tests.cpp"
    "src/httpreplylistener_tests.cpp"
    "src/urlencodedrequest_tests.cpp"
    "src/config_tests.cpp"
    "src/defaultsettings_tests.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PRIVATE $<TARGET_PROPERTY:tests,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:http,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:config,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:settings,INCLUDE_DIRECTORIES>
)

target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE $ENV{GTEST_DIR}/include
)

find_library(GTEST_LIB gtest $ENV{GTEST_DIR}/lib)
find_library(GMOCK_LIB gmock $ENV{GTEST_DIR}/lib)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    ${GTEST_LIB}
    ${GMOCK_LIB}
    tests
    utils
    http
    config
    settings
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
