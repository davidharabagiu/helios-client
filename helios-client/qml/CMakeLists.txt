cmake_minimum_required(VERSION 3.1)
project(qml)

set(QML_MODULE_BASE_NAME Helios)

set(QML_FILES
    "main.qml"
    "ApplicationWorkflow.qml"
)

add_custom_target(${PROJECT_NAME} SOURCES ${QML_FILES})

install(FILES ${QML_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/qml)
