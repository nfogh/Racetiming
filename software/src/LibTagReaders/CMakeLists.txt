set(MODULE_NAME TagReaders)
find_package(QT NAMES Qt6 Qt5 COMPONENTS SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SerialPort REQUIRED)

add_library(${MODULE_NAME} STATIC
        src/m6enano.cpp
        src/m6enano.h

        src/TagReaders.cpp

        include/TagReaders/TagReaders.h
        include/TagReaders/ITagReader.h
)
add_library(${MODULE_NAME}::Core ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
    PUBLIC
        include
    PRIVATE
        src
        include/${MODULE_NAME}
)

target_link_libraries(${MODULE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
