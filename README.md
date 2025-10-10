# WorkDayTracker

A desktop application for tracking work hours and managing your workday efficiently. Built with C++ and Qt6, this application helps you monitor your work time, track breaks, and visualize your productivity.

## Features

- **Time Tracking**: Start, pause, resume, and stop your work timer
- **Workday Management**: Set daily work hour goals and break requirements
- **Visual Feedback**: Real-time display of elapsed time and remaining work time
- **Finish Time Estimation**: Predicts when your workday will end based on current progress
- **Statistics**: View your work patterns and productivity metrics
- **Data Visualization**: Charts showing your daily work durations
- **System Tray Integration**: Minimize to system tray and receive notifications
- **Persistent Storage**: Automatically saves your work sessions and settings
- **Cross-platform**: Works on Windows, macOS, and Linux

## Screenshots

![Main Window](docs/screenshots/main_window.png)
![Statistics](docs/screenshots/statistics.png)
![Settings](docs/screenshots/settings.png)

## Installation

### Prerequisites

- CMake 3.22 or higher
- Qt 6.9.2 or higher with Widgets and Charts modules
- C++17 compatible compiler (MSVC 2022, GCC 7+, Clang 6+)

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/barmaglote-extra/work-time-tracker.git
   cd WorkDayTracker
   ```

2. Configure with CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```

### Windows

On Windows, you can use Visual Studio or build from the command line:

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

1. Launch the application
2. Set your daily work goals and break requirements in the Settings menu
3. Click "Start" to begin tracking your work time
4. Use "Pause" during breaks and "Resume" when returning to work
5. Click "Stop" at the end of your workday
6. View statistics and charts to analyze your productivity

## Configuration

The application stores settings in `settings.json` with the following structure:

```json
{
  "1": {"workSeconds": 32400, "breakSeconds": 2700},
  "2": {"workSeconds": 32400, "breakSeconds": 2700},
  "3": {"workSeconds": 32400, "breakSeconds": 2700},
  "4": {"workSeconds": 32400, "breakSeconds": 2700},
  "5": {"workSeconds": 32400, "breakSeconds": 2700},
  "6": {"workSeconds": 0, "breakSeconds": 0},
  "7": {"workSeconds": 0, "breakSeconds": 0}
}
```

Where:
- Keys 1-7 represent Monday through Sunday
- `workSeconds`: Required work time in seconds (32400 = 9 hours)
- `breakSeconds`: Minimum break time in seconds (2700 = 45 minutes)

## Data Files

- `state.json`: Stores the current timer state and events
- `daily_durations.json`: Records daily work durations for statistics
- `settings.json`: User configuration for work hours and breaks

## Documentation

- [Usage Guide](docs/USAGE.md) - Instructions on how to use the application
- [Building and Deployment](docs/BUILDING.md) - Instructions for building from source and deployment
- [GitHub Actions Troubleshooting](docs/GITHUB_ACTIONS.md) - Solutions for common GitHub Actions issues
- [Architecture](docs/ARCHITECTURE.md) - Technical documentation of the application architecture
- [Contributing](CONTRIBUTING.md) - Guidelines for contributing to the project

## Continuous Integration

This project uses GitHub Actions for continuous integration. The workflow builds the application on Windows, macOS, and Linux platforms to ensure cross-platform compatibility.

If you encounter any issues with the GitHub Actions workflow, please refer to our [GitHub Actions Troubleshooting Guide](docs/GITHUB_ACTIONS.md) for common solutions.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on the process for submitting pull requests.

Please also read our [Code of Conduct](CODE_OF_CONDUCT.md) to understand our community standards.

## License

This project is licensed under the AGPL-3.0 License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Icons by [Icons Icons](https://icon-icons.com/ru/)
- Built with [Qt6](https://www.qt.io/)