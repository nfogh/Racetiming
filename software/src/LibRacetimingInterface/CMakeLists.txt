set(MODULE_NAME RacetimingInterface)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

add_library(${MODULE_NAME} STATIC
    src/WebRacetimingInterface.cpp
    src/WebRacetimingInterface.h
    src/TestRacetimingInterface.cpp
    src/TestRacetimingInterface.h
    src/LocalRacetimingInterface.cpp
    src/LocalRacetimingInterface.h

    src/RacetimingInterface.cpp

    include/RacetimingInterface/RacetimingInterface.h
    include/RacetimingInterface/IRacetimingInterface.h
    include/RacetimingInterface/RacetimingTypes.h
)
add_library(${MODULE_NAME}::Core ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
        include
	PRIVATE
        src
        include/RacetimingInterface
)

target_link_libraries(${MODULE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Network)
