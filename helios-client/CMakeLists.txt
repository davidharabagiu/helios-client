cmake_minimum_required(VERSION 3.1)

project(helios-client)

set(CMAKE_SCRIPTS ${CMAKE_SOURCE_DIR}/cmake)

option(WITH_TESTS "Tests enabled" ON)

add_subdirectory(helios)
add_subdirectory(qml)
add_subdirectory(http)
add_subdirectory(utils)
add_subdirectory(apicalls)
add_subdirectory(config)
if (WITH_TESTS)
    add_subdirectory(tests)
endif()
