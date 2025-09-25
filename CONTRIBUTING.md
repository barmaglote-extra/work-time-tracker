# Contributing to WorkDayTracker

Thank you for your interest in contributing to WorkDayTracker! This document provides guidelines and information to help you contribute effectively.

## Table of Contents

- [Getting Started](#getting-started)
- [Project Structure](#project-structure)
- [Development Guidelines](#development-guidelines)
- [Code Style](#code-style)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/barmaglote-extra/work-time-tracker.git`
3. Create a new branch for your feature or bug fix: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Commit your changes with a descriptive commit message
6. Push to your fork: `git push origin feature/your-feature-name`
7. Create a Pull Request

## Project Structure

```
WorkDayTracker/
├── src/                    # Source code
│   ├── components/         # UI components
│   ├── models/             # Data models
│   ├── states/             # Application state management
│   ├── styles/             # UI styling
│   ├── utils/              # Utility functions
│   ├── windows/            # Application windows
│   └── main.cpp            # Application entry point
├── resources/              # Application resources (icons, etc.)
├── CMakeLists.txt          # Build configuration
├── resources.qrc           # Qt resource file
├── settings.json           # Default settings
├── state.json              # Runtime state data
├── daily_durations.json    # Daily work duration records
├── .gitignore              # Git ignore rules
├── LICENSE                 # License information
└── README.md               # Project documentation
```

## Development Guidelines

### Components

The application follows a component-based architecture where each UI element is encapsulated in its own class:

- **Base Components**: Abstract base classes that define interfaces
- **Concrete Components**: Implementations of the base components
- **Windows**: Main application windows that compose components

### State Management

The application uses a central state management system implemented in [MainWindowState](src/states/main_window_state/MainWindowState.h). All components that need to access or modify application state should interact with this class.

### Data Persistence

The application automatically saves and loads data using JSON files:
- `state.json`: Current timer state and events
- `daily_durations.json`: Historical work duration data
- `settings.json`: User configuration

## Code Style

### C++ Standards

- Follow C++17 best practices
- Use modern C++ features where appropriate
- Prefer smart pointers over raw pointers
- Use const-correctness consistently

### Qt Specifics

- Follow Qt naming conventions (camelCase for methods and variables)
- Use Qt's parent-child memory management
- Prefer Qt containers over STL containers when interfacing with Qt APIs
- Use signals and slots for component communication

### Naming Conventions

- Classes: PascalCase (`MainWindow`, `TimerWidget`)
- Methods: camelCase (`startTimer()`, `updateDisplay()`)
- Variables: camelCase (`timerValue`, `isRunning`)
- Constants: UPPER_SNAKE_CASE (`MAX_DURATION`, `DEFAULT_BREAK_TIME`)

### File Organization

- Header files (.h) contain declarations
- Implementation files (.cpp) contain definitions
- Each class should be in its own file
- Related components should be grouped in subdirectories

## Pull Request Process

1. Ensure your code follows the project's coding standards
2. Write clear, descriptive commit messages
3. Include tests for new functionality if applicable
4. Update documentation as needed
5. Verify that all tests pass
6. Submit a pull request with a clear description of the changes

### Pull Request Guidelines

- Keep PRs focused on a single feature or bug fix
- Include a clear description of what changed and why
- Reference any related issues
- Ensure your branch is up to date with the main branch

## Reporting Issues

When reporting issues, please include:

1. A clear and descriptive title
2. Steps to reproduce the issue
3. Expected behavior
4. Actual behavior
5. System information (OS, Qt version, compiler)
6. Screenshots if applicable
7. Any relevant error messages or logs

### Bug Reports

For bug reports, please use the bug report template:

```markdown
## Description
[Description of the bug]

## Steps to Reproduce
1. [First Step]
2. [Second Step]
3. [And so on...]

## Expected Behavior
[What you expected to happen]

## Actual Behavior
[What actually happened]

## Environment
- OS: [e.g., Windows 10, macOS 12.0, Ubuntu 20.04]
- Qt Version: [e.g., 6.2.4]
- Compiler: [e.g., MSVC 2022, GCC 11.2]
- WorkDayTracker Version: [if applicable]

## Additional Context
[Add any other context about the problem here]
```

Thank you for contributing to WorkDayTracker!