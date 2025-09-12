cmake_minimum_required(VERSION 3.22)
project(WorkDayTracker LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # use "clean" C++17
add_executable(WorkDayTracker src/main.cpp)