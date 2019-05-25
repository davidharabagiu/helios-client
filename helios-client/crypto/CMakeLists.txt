cmake_minimum_required(VERSION 3.1)

project(crypto)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/public/cipher.h"
    "inc/public/cipherfactory.h"
    "inc/public/rsa.h"
    "inc/private/rsaimpl.h"
    "inc/public/random.h"
    "inc/public/randomfactory.h"
    "inc/private/randomfactoryimpl.h"
    "src/randomfactoryimpl.cpp"
    "inc/private/isaac/standard.h"
    "inc/private/isaac/isaac64.h"
    "inc/private/isaac/isaac64wrapper.h"
    "src/isaac/isaac64.c"
    "src/isaac/isaac64wrapper.cpp"
    "src/rsaimpl.cpp"
    "inc/private/cipherfactoryimpl.h"
    "src/cipherfactoryimpl.cpp"
    "inc/private/aes/aescipher.h"
    "src/aes/aescipher.cpp"
    "inc/private/aes/commondefs.h"
    "inc/private/aes/utils.h"
    "inc/private/aes/tables.h"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/private
    PRIVATE $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE $ENV{BIGINT_DIR}/include
)

find_library(BIGINT_LIB bigint $ENV{BIGINT_DIR}/lib)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    ${BIGINT_LIB}
    utils
)
