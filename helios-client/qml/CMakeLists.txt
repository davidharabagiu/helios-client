cmake_minimum_required(VERSION 3.1)
project(qml)

#set(QML_IMPORT_PATH ${CMAKE_INSTALL_PREFIX}/qml)

set(QML_FILES
    "main.qml"
    "ApplicationWorkflow.qml"
    "Style.qml"
    "DarkModeStyle.qml"
    "HButton.qml"
    "HTextInput.qml"
    "HCheckBox.qml"
    "LoginScreen.qml"
    "RegisterScreen.qml"
)

add_custom_target(${PROJECT_NAME} SOURCES ${QML_FILES})

install(FILES ${QML_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/qml)
