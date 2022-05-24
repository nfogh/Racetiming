find_package(QT NAMES Qt6 Qt5 COMPONENTS SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SerialPort REQUIRED)

add_library(RFIDRW STATIC
        src/m6enano.cpp
        src/m6enano.h

        src/RFIDRW.cpp

        include/RFIDRW/RFIDRW.h
        include/RFIDRW/IRFIDRW.h
)

target_include_directories(RFIDRW
	PUBLIC
        include
	PRIVATE
        src
        include/RFIDRW
)

target_link_libraries(RFIDRW PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
