cmake_minimum_required(VERSION 3.0)
project(p04QT)

add_compile_options(-std=c++11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)

set(p04_SRCS main.cpp widget.cpp)
qt5_wrap_ui(p04_UIS widget.ui)

add_executable(p04 ${p04_SRCS} ${p04_UIS})
target_link_libraries(p04 Qt5::Widgets Qt5::Core)
set_property(TARGET p04 PROPERTY CXX_STANDARD 11)
