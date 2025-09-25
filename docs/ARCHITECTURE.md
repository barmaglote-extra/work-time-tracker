# WorkDayTracker Architecture

This document describes the architecture of the WorkDayTracker application, including its components, design patterns, and data flow.

## Overview

WorkDayTracker is a desktop application built with C++ and Qt6 that follows a component-based architecture with a central state management system. The application is designed to be modular, maintainable, and extensible.

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                      Main Application                       │
├─────────────────────────────────────────────────────────────┤
│                    MainWindow (Container)                   │
├─────────────────────────────────────────────────────────────┤
│  Menu  │  Central Widget  │  Status Bar  │  System Tray    │
├─────────────────────────────────────────────────────────────┤
│                    Application State                        │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. Windows

#### MainWindow
The main application window that serves as a container for all other components. It manages the overall layout and coordination between components.

#### SettingsWindow
A separate window for configuring work hours and break requirements.

#### AboutWindow
Displays application information and version details.

#### BaseWindow
An abstract base class that defines the common interface for all windows.

### 2. Components

#### TimerWidget
Displays the current elapsed time and provides visual feedback about the timer status.

#### ControlPanel
Contains the main control buttons (Start, Pause, Resume, Stop) for the timer.

#### FinishTimeWidget
Displays the estimated finish time based on current progress and settings.

#### StatsWidget
Shows various statistics about work patterns and productivity metrics.

#### DailyChartWidget
Visualizes daily work durations using Qt Charts.

#### TimerChart
A specialized chart component for displaying timer-related data.

#### TrayHelper
Manages system tray integration and notifications.

#### Abstract Components
Abstract base classes that define interfaces for various component types:
- AbstractMenu
- AbstractCentralWidget
- AbstractStatusBar

### 3. State Management

#### MainWindowState
The central state management class that:
- Manages the timer state (running, paused, stopped)
- Stores timer events and history
- Calculates finish times
- Manages work hour settings
- Handles data persistence

### 4. Models

#### TimerEvent
Represents a single timer event (start, pause, resume, stop) with timestamp information.

### 5. Utilities

#### TimeCalculator
Provides utility functions for time calculations, including finish time estimation.

### 6. Styles

Custom styling components for consistent UI appearance:
- ButtonStyles
- FinishTimeStyles
- ProgressbarStyles
- SettingsStyles
- StatsStyles
- TrayStyles

## Data Flow

### 1. User Interaction
```
User Action → Component → MainWindowState → Update Components → UI Update
```

### 2. Timer Updates
```
Timer Tick → MainWindowState → Update Components → UI Update
```

### 3. Data Persistence
```
State Change → MainWindowState → Save to JSON Files
```

## Design Patterns

### 1. Model-View-Controller (MVC)
The application follows MVC principles with:
- **Model**: MainWindowState manages all application data
- **View**: UI components display data
- **Controller**: MainWindowState handles user interactions and business logic

### 2. Observer Pattern
Components observe state changes through Qt's signal and slot mechanism:
- MainWindowState emits signals when data changes
- Components connect to these signals to update their display

### 3. Singleton Pattern
The MainWindowState acts as a singleton, providing a single source of truth for application state.

### 4. Component Pattern
UI elements are encapsulated in reusable components that can be composed together.

## Data Persistence

The application uses JSON files for data persistence:

### state.json
Stores the current timer state and events:
- Timer value
- Timer status
- Event history
- Last saved timestamp

### daily_durations.json
Records daily work durations for statistics:
- Date
- Work duration in seconds

### settings.json
User configuration for work hours:
- Work seconds per day
- Minimum break seconds per day

## Threading Model

The application is single-threaded and uses Qt's event loop for asynchronous operations:
- QTimer for periodic updates
- Signals and slots for inter-component communication
- Non-blocking file I/O operations

## Error Handling

The application handles errors gracefully:
- File I/O errors are caught and logged
- Invalid data is handled with default values
- UI components provide feedback for invalid operations

## Extensibility

The component-based architecture makes the application easy to extend:
- New components can be added by inheriting from abstract base classes
- New features can be implemented by extending MainWindowState
- UI can be modified by creating new component implementations

## Dependencies

### Qt6 Modules
- QtWidgets: UI components and window management
- QtCharts: Data visualization
- QtCore: Core functionality, signals and slots, JSON handling

### Standard Libraries
- C++ Standard Library
- STL containers where appropriate

## Build System

The application uses CMake as its build system with the following key features:
- Automatic Qt MOC, UIC, and RCC processing
- Cross-platform compilation support
- Resource compilation for icons and assets

## Testing

The application includes unit tests for critical components:
- Time calculations
- State management
- Data persistence
- Event handling

## Performance Considerations

- Efficient data structures for storing timer events
- Periodic data saving to avoid data loss
- Lazy loading of historical data
- Optimized UI updates using Qt's signal-slot mechanism