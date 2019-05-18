cmake_minimum_required(VERSION 3.1)

project(testapp)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/rpath_config.cmake REQUIRED)

add_executable(${PROJECT_NAME}
    "main.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE $<TARGET_PROPERTY:crypto,INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    crypto
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
