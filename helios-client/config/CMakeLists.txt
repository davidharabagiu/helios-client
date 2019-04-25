cmake_minimum_required(VERSION 3.1)
project(config)

SET(CONFIG_FILE "config.json")

add_custom_target(${PROJECT_NAME} SOURCES ${CONFIG_FILE})

# Install
install(FILES ${CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX})
