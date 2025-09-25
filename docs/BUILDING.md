# Building and Deploying WorkDayTracker

This guide explains how to build the WorkDayTracker application from source and deploy it.

## Prerequisites

Before building the application, ensure you have the following installed:

1. **CMake** (version 3.22 or higher)
2. **Qt6** (version 6.9.2 or higher) with the following modules:
   - QtWidgets
   - QtCharts
3. **C++ Compiler** with C++17 support:
   - MSVC 2022 (Windows)
   - GCC 7+ (Linux)
   - Clang 6+ (macOS)
4. **Git** (for cloning the repository)

## Getting the Source Code

Clone the repository from GitHub:

```bash
git clone https://github.com/barmaglote-extra/work-time-tracker.git
cd WorkDayTracker
```

## Building the Application

### Windows (Visual Studio)

1. Open a Developer Command Prompt for Visual Studio
2. Navigate to the project directory
3. Create a build directory and configure with CMake:

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.2/msvc2022_64"
```

4. Build the project:

```bash
cmake --build . --config Release
```

### Linux/macOS

1. Open a terminal
2. Navigate to the project directory
3. Create a build directory and configure with CMake:

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.9.2/gcc_64"
```

4. Build the project:

```bash
make
```

### CMake Configuration Options

The following CMake options are available:

- `CMAKE_BUILD_TYPE`: Build type (Debug, Release, RelWithDebInfo, MinSizeRel)
- `CMAKE_PREFIX_PATH`: Path to Qt installation
- `CMAKE_INSTALL_PREFIX`: Installation directory

## Project Structure

The CMake build system is organized as follows:

```
CMakeLists.txt          # Main CMake configuration
src/                    # Source code
├── components/         # UI components
├── models/             # Data models
├── states/             # Application state
├── styles/             # UI styling
├── utils/              # Utility functions
├── windows/            # Application windows
└── main.cpp            # Entry point
resources/              # Application resources
├── icons/              # SVG icons
└── resources.qrc       # Qt resource file
```

## Qt Resource System

The application uses Qt's resource system to bundle icons and other assets. The resources are defined in `resources.qrc` and compiled into the application binary.

To add new resources:
1. Add the files to the `resources/` directory
2. Update `resources.qrc` to include the new files
3. Rebuild the project

## Deployment

### Windows Deployment

To create a standalone Windows executable:

1. Build the application in Release mode
2. Use `windeployqt` to copy required Qt libraries:

```bash
windeployqt --dir ./deploy WorkDayTracker.exe
```

3. Copy the following files to the deployment directory:
   - `WorkDayTracker.exe`
   - Qt runtime libraries (copied by windeployqt)
   - `settings.json` (default configuration)
   - Any other required DLLs

### Linux Deployment

To create a standalone Linux executable:

1. Build the application
2. Identify required Qt libraries using `ldd`:

```bash
ldd WorkDayTracker
```

3. Create an AppImage or package with the required libraries
4. Include the `settings.json` file in the package

### macOS Deployment

To create a standalone macOS application:

1. Build the application
2. Use `macdeployqt` to bundle Qt libraries:

```bash
macdeployqt WorkDayTracker.app
```

3. Create a DMG installer if desired

## Installation

### Creating an Installer

#### Windows

You can create an installer using tools like:
- Inno Setup
- NSIS
- WiX Toolset

A basic Inno Setup script might look like:

```ini
[Setup]
AppName=WorkDayTracker
AppVersion=1.0
DefaultDirName={pf}\WorkDayTracker
DefaultGroupName=WorkDayTracker
UninstallDisplayIcon={app}\WorkDayTracker.exe
Compression=lzma2
SolidCompression=yes

[Files]
Source: "WorkDayTracker.exe"; DestDir: "{app}"
Source: "Qt6Core.dll"; DestDir: "{app}"
Source: "Qt6Gui.dll"; DestDir: "{app}"
Source: "Qt6Widgets.dll"; DestDir: "{app}"
Source: "settings.json"; DestDir: "{app}"

[Icons]
Name: "{group}\WorkDayTracker"; Filename: "{app}\WorkDayTracker.exe"
Name: "{group}\{cm:UninstallProgram,WorkDayTracker}"; Filename: "{uninstallexe}"
```

#### Linux

For Linux, you can create packages for different distributions:

**Debian/Ubuntu (.deb package):**

Create the directory structure:
```
workdaytracker/
├── usr/
│   ├── bin/
│   │   └── WorkDayTracker
│   └── share/
│       ├── applications/
│       │   └── workdaytracker.desktop
│       └── workdaytracker/
│           └── settings.json
└── DEBIAN/
    └── control
```

**Red Hat/Fedora (.rpm package):**

Use `rpmbuild` with an appropriate spec file.

#### macOS

Create a DMG file using `hdiutil`:

```bash
hdiutil create -volname "WorkDayTracker" -srcfolder ./WorkDayTracker.app -ov -format UDZO WorkDayTracker.dmg
```

## Continuous Integration

The project can be built using CI systems like:

### GitHub Actions

Example workflow file (`.github/workflows/build.yml`):

```yaml
name: Build
on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]

    steps:
    - uses: actions/checkout@v2

    - name: Install Qt
      uses: jurplel/install-qt-action@v2
      with:
        version: '6.9.2'

    - name: Configure CMake
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release

    - name: Build
      run: cmake --build build --config Release
```

## Troubleshooting

### Common Build Issues

#### Qt Not Found

If CMake cannot find Qt:

1. Verify Qt is installed
2. Set `CMAKE_PREFIX_PATH` to your Qt installation directory:

```bash
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.9.2/gcc_64"
```

#### Missing Qt Modules

If you get errors about missing Qt modules:

1. Ensure you installed the required modules (Widgets, Charts)
2. Verify the modules are available in your Qt installation

#### Compiler Not Found

If CMake cannot find a compiler:

1. On Windows, ensure you're using a Developer Command Prompt
2. On Linux/macOS, ensure gcc/clang is installed
3. Set the compiler explicitly if needed:

```bash
export CC=gcc
export CXX=g++
cmake ..
```

### Runtime Issues

#### Missing Libraries

If the application fails to start with missing library errors:

1. On Windows, run `windeployqt` to copy required libraries
2. On Linux, check library paths with `ldd`
3. On macOS, use `macdeployqt` to bundle libraries

#### Icons Not Displaying

If icons are not displaying:

1. Verify the resource file (`resources.qrc`) includes all icons
2. Ensure the application can access the resource file
3. Check that icon paths in the code match the resource file

## Versioning

The application follows semantic versioning (MAJOR.MINOR.PATCH):

- MAJOR version for incompatible API changes
- MINOR version for backward-compatible functionality
- PATCH version for backward-compatible bug fixes

Update the version in:
1. CMakeLists.txt: `project(WorkDayTracker VERSION x.y.z)`
2. Application about dialog
3. Documentation

## Release Process

1. Update version numbers
2. Update CHANGELOG.md
3. Create a release branch
4. Build for all target platforms
5. Test all builds
6. Create installers/packages
7. Create a GitHub release
8. Upload binaries and installers
9. Update documentation if needed