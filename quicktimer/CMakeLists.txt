cmake_minimum_required(VERSION 3.10)

project(quicktimer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.0\\msvc2019_64\\")

find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE
	main.cpp
	quicktimer.cpp
	Settings.cpp
	TimerLabel.cpp
	utils.cpp)

set(HEADER
    quicktimer.h
    Settings.h
    TimerLabel.h
	utils.h)

set(UI
	quicktimer.ui)

set(QRC
	quicktimer.qrc)

add_executable(quicktimer ${SOURCE} ${HEADER} ${UI} ${RCC})
target_link_libraries(quicktimer Qt5::Widgets)

