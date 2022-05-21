find_package(QT NAMES Qt6 Qt5 COMPONENTS SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SerialPort REQUIRED)

add_library(RFIDReaders STATIC
	src/m6erfidreader.cpp
	src/m6erfidreader.h

        src/RFIDReaders.cpp

	include/RFIDReaders/RFIDReaders.h
	include/RFIDReaders/IRFIDReader.h
)

target_include_directories(RFIDReaders
	PUBLIC
        include
	PRIVATE
        src
        include/RFIDReaders
)

target_link_libraries(RFIDReaders PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
