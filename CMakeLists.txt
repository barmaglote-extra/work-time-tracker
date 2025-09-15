cmake_minimum_required(VERSION 3.22)
project(WorkDayTracker LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # use "clean" C++17


set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/msvc2022_64")
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(WorkDayTracker ${SOURCES})
target_link_libraries(WorkDayTracker PRIVATE Qt6::Widgets Qt6::Charts)
target_include_directories(WorkDayTracker PRIVATE src)

set_target_properties(WorkDayTracker PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

qt_add_resources(RESOURCES_ADDED resources.qrc)
target_sources(WorkDayTracker PRIVATE ${RESOURCES_ADDED})