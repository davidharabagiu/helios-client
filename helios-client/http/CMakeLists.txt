cmake_minimum_required(VERSION 3.1)

project(http)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "src/httprequestmanagerimpl.cpp"
    "src/httpreplylistener.cpp"
    "src/urlencodedrequestimpl.cpp"
    "src/formdatarequestimpl.cpp"
    "src/httprequestfactoryimpl.cpp"
    "inc/public/httprequestmanager.h"
    "inc/public/httpstatus.h"
    "inc/public/urlencodedrequest.h"
    "inc/public/formdatarequest.h"
    "inc/public/httprequestfactory.h"
    "inc/private/httprequestmanagerimpl.h"
    "inc/private/httpreplylistener.h"
    "inc/private/urlencodedrequestprivate.h"
    "inc/private/formdatarequestprivate.h"
    "inc/private/urlencodedrequestimpl.h"
    "inc/private/formdatarequestimpl.h"
    "inc/private/httprequestfactoryimpl.h"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:async,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    utils
    async
)
