cmake_minimum_required(VERSION 3.1)

project(helios-client)

set(CMAKE_SCRIPTS ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(helios)
add_subdirectory(qml)
add_subdirectory(http)
add_subdirectory(utils)
add_subdirectory(apicalls)
