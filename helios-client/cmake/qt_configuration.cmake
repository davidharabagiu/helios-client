find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Quick REQUIRED)

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Quick_DEFINITIONS})

include_directories(SYSTEM ${Qt5Core_INCLUDE_DIRS})
include_directories(SYSTEM ${Qt5Quick_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT5_LIBS
    ${Qt5Core_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
)
