cmake_minimum_required(VERSION 3.22)
project(WorkDayTracker LANGUAGES CXX)

# --- Set the C++ standard to C++17 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use clean C++17 without compiler-specific extensions

# --- Specify the Qt installation path and find required components ---
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/msvc2022_64")
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

# --- Collect all .cpp source files ---
file(GLOB_RECURSE SOURCES src/*.cpp)

# --- Collect all .h header files (needed for AUTOMOC to process Q_OBJECT) ---
file(GLOB_RECURSE HEADERS src/*.h)

# --- Add the executable target and include Windows resource file for icon ---
add_executable(WorkDayTracker
    ${SOURCES}    # All source files
    ${HEADERS}    # All header files with Q_OBJECT macros
    app.rc        # Windows resource file containing the application icon
)

# --- On Windows, hide the console window in Release builds ---
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(WorkDayTracker PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# --- Enable automatic Qt features ---
set_target_properties(WorkDayTracker PROPERTIES
    AUTOMOC ON     # Automatically run moc on headers with Q_OBJECT
    AUTOUIC ON     # Automatically process .ui files
    AUTORCC ON     # Automatically process Qt resource files (.qrc)
)

# --- Link required Qt libraries ---
target_link_libraries(WorkDayTracker PRIVATE Qt6::Widgets Qt6::Charts)

# --- Include source directories for header files ---
target_include_directories(WorkDayTracker PRIVATE src)

# --- Add Qt resources from .qrc file ---
qt_add_resources(RESOURCES_ADDED resources.qrc)
target_sources(WorkDayTracker PRIVATE ${RESOURCES_ADDED})
