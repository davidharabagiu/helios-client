cmake_minimum_required(VERSION 3.1)

project(httptests)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/rpath_config.cmake REQUIRED)

add_executable(${PROJECT_NAME}
    "src/main.cpp"
    "src/http_tests.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE $<TARGET_PROPERTY:tests,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:http,INCLUDE_DIRECTORIES>
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
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
