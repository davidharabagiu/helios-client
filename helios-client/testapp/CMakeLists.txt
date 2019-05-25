cmake_minimum_required(VERSION 3.1)

project(testapp)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/rpath_config.cmake REQUIRED)

add_executable(${PROJECT_NAME}
    "main.cpp"
)

target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE $ENV{CRYPTOPP_DIR}/include/cryptopp
    PRIVATE $<TARGET_PROPERTY:crypto,INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:utils,INCLUDE_DIRECTORIES>
)

find_library(CRYPTOPP_LIB cryptopp $ENV{CRYPTOPP_DIR}/lib)

target_link_libraries(${PROJECT_NAME}
    ${CRYPTOPP_LIB}
    crypto
    utils
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
