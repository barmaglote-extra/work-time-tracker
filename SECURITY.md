# Security Policy

## Supported Versions

Currently, only the latest version of WorkDayTracker is supported with security updates.

| Version | Supported          |
| ------- | ------------------ |
| 1.x.x   | :white_check_mark: |
| < 1.0   | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability within WorkDayTracker, please send an email to [your-email@example.com] with a detailed description of the issue.

Please do not create public GitHub issues for security vulnerabilities.

We will acknowledge your email within 48 hours and send a more detailed response within 72 hours indicating the next steps in handling your report.

After the initial reply to your report, we will endeavor to keep you informed of the progress towards a fix and full announcement, and may ask for additional information or guidance.

## Security Considerations

WorkDayTracker is a desktop application that:

- Stores data locally in JSON files
- Does not transmit data over networks
- Does not require internet access for core functionality
- Uses Qt6 framework with built-in security features

The application follows security best practices by:

- Using modern C++ standards
- Avoiding unsafe memory operations
- Validating input data
- Following Qt security guidelines