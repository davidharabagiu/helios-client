cmake_minimum_required(VERSION 3.1)

project(http)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "src/httprequestmanagerimpl.cpp"
    "src/httpreplylistener.cpp"
    "inc/public/httprequestmanager.h"
    "inc/public/httpstatus.h"
    "inc/private/httprequestmanagerimpl.h"
    "inc/private/httpreplylistener.h"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
)
