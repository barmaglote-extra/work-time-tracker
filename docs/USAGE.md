# WorkDayTracker Usage Guide

This guide explains how to use the WorkDayTracker application effectively.

## Getting Started

When you first launch WorkDayTracker, you'll see the main window with the timer interface. The application is designed to be simple and intuitive to use.

## Main Interface

The main window consists of several key components:

1. **Timer Display**: Shows the current elapsed time
2. **Control Buttons**: Start, Pause, Resume, and Stop the timer
3. **Finish Time Display**: Shows the estimated time when your workday will end
4. **Menu Bar**: Access to settings, statistics, and other features
5. **Status Bar**: Shows application status and notifications

## Using the Timer

### Starting Your Workday

1. Click the "Start" button to begin tracking your work time
2. The timer will begin counting seconds
3. The estimated finish time will be calculated based on your settings

### Taking Breaks

1. When you need a break, click the "Pause" button
2. When you return from your break, click the "Resume" button
3. The application will track your break time

### Ending Your Workday

1. When you're finished working, click the "Stop" button
2. Your work session data will be saved automatically
3. The timer will reset for the next workday

## Menu Options

### File Menu

- **Exit**: Close the application

### View Menu

- **Statistics**: View work statistics and productivity metrics
- **Daily Chart**: See a chart of your daily work durations
- **Settings**: Configure work hours and break requirements

### Help Menu

- **About**: View application information and version

## Configuring Settings

You can customize your workday requirements through the Settings window:

1. Open the Settings window from the View menu
2. Set your required work hours for each day of the week
3. Set your minimum break requirements for each day
4. Changes are saved automatically

The settings are stored in the `settings.json` file with the following structure:

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

## Viewing Statistics

The Statistics view shows various metrics about your work patterns:

- Total work time
- Average work time per day
- Break time statistics
- Productivity trends

## Daily Chart

The Daily Chart view visualizes your work durations over time, helping you identify patterns in your work habits.

## System Tray Integration

WorkDayTracker can be minimized to the system tray:

- Click the minimize button to send the application to the system tray
- Double-click the tray icon to restore the application
- Right-click the tray icon for quick access to common functions

## Data Files

The application stores data in several JSON files:

- `state.json`: Current timer state and events
- `daily_durations.json`: Historical work duration data
- `settings.json`: User configuration

These files are automatically managed by the application and should not be modified while the application is running.

## Notifications

The application will notify you when your workday is about to end, based on your configured work hours and current progress.

## Troubleshooting

### Timer Not Starting

- Ensure you haven't already started the timer
- Check that the application has permission to run

### Incorrect Finish Time

- Verify your settings in the Settings window
- Make sure your system clock is accurate

### Missing Data

- Check that the data files (`state.json`, `daily_durations.json`) exist and are writable
- Restart the application to reload data

## Tips for Effective Use

1. **Be Consistent**: Try to start and stop your timer at the same times each day
2. **Take Regular Breaks**: Use the pause feature to track your breaks
3. **Review Statistics**: Regularly check your statistics to identify patterns
4. **Adjust Settings**: Modify your work hour requirements as needed