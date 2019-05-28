cmake_minimum_required(VERSION 3.1)

project(apicalls)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/apicalldefs.h"
    "inc/public/userapicaller.h"
    "inc/public/fileapicaller.h"
    "inc/public/notificationsapicaller.h"
    "inc/private/userapicallerimpl.h"
    "src/userapicallerimpl.cpp"
    "inc/private/fileapicallerimpl.h"
    "src/fileapicallerimpl.cpp"
    "inc/private/notificationsapicallerimpl.h"
    "src/notificationsapicallerimpl.cpp"
    "inc/private/apicall.h"
    "inc/private/apicallvisitor.h"
    "inc/private/apicallvisitor.tpp"
    "src/apicallvisitor.cpp"
    "inc/private/registerusercall.h"
    "src/registerusercall.cpp"
    "inc/private/logincall.h"
    "src/logincall.cpp"
    "inc/private/logoutcall.h"
    "src/logoutcall.cpp"
    "inc/private/createdirectorycall.h"
    "src/createdirectorycall.cpp"
    "inc/private/beginuploadcall.h"
    "src/beginuploadcall.cpp"
    "inc/private/uploadcall.h"
    "src/uploadcall.cpp"
    "inc/private/begindownloadcall.h"
    "src/begindownloadcall.cpp"
    "inc/private/downloadcall.h"
    "src/downloadcall.cpp"
    "inc/private/endtransfercall.h"
    "src/endtransfercall.cpp"
    "inc/private/getfilesizecall.h"
    "src/getfilesizecall.cpp"
    "inc/private/listcall.h"
    "src/listcall.cpp"
    "inc/private/removecall.h"
    "src/removecall.cpp"
    "inc/private/movecall.h"
    "src/movecall.cpp"
    "inc/private/checktokencall.h"
    "src/checktokencall.cpp"
    "inc/private/isdircall.h"
    "src/isdircall.cpp"
    "inc/private/getuserkeycall.h"
    "src/getuserkeycall.cpp"
    "inc/private/setuserkeycall.h"
    "src/setuserkeycall.cpp"
    "inc/private/notificationscall.h"
    "src/notificationscall.cpp"
    "inc/private/dismissnotificationcall.h"
    "src/dismissnotificationcall.cpp"
    "inc/private/dismissallnotificationscall.h"
    "src/dismissallnotificationscall.cpp"
    "inc/private/sharekeycall.h"
    "src/sharekeycall.cpp"
    "inc/private/acceptkeycall.h"
    "src/acceptkeycall.cpp"
    "inc/private/sharecall.h"
    "src/sharecall.cpp"
    "inc/private/acceptfilecall.h"
    "src/acceptfilecall.cpp"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE $<TARGET_PROPERTY:http,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
    PRIVATE $<TARGET_PROPERTY:config,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    http
    utils
    config
)
