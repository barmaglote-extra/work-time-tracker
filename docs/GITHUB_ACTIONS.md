# GitHub Actions Troubleshooting

This guide provides solutions for common issues encountered when running GitHub Actions for the WorkDayTracker project.

## Common Issues and Solutions

### 1. Qt Installation Failures

**Error Message**: `Qt version not found` or `Module not available`

**Solution**:
- Check that the Qt version specified in the workflow (6.9.2) is still available
- Verify that all required modules are correctly specified
- Try using a more recent Qt version if the specified one is no longer available

```yaml
- name: Install Qt
  uses: jurplel/install-qt-action@v3
  with:
    version: 6.9.2  # Try updating to a newer version like 6.10.0
    modules: 'qtcharts'
```

### 2. CMake Configuration Failures

**Error Message**: `Could not find Qt6` or `Qt6Config not found`

**Solution**:
- Ensure the CMAKE_PREFIX_PATH is correctly set to the Qt installation directory
- Verify that Qt was installed successfully in previous steps
- Check that the required Qt components are installed

```yaml
- name: Configure CMake (Windows)
  if: runner.os == 'Windows'
  run: cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$env:QT_ROOT_DIR

- name: Configure CMake (Linux/macOS)
  if: runner.os != 'Windows'
  run: cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$QT_ROOT_DIR
```

### 3. Build Failures

**Error Message**: `Build failed` or compiler errors

**Solution**:
- Check that all source files are included in the repository
- Verify that the C++ standard is correctly set (C++17)
- Ensure that all required Qt libraries are linked properly

### 4. Artifact Upload Issues

**Error Message**: `No files found` or `Upload failed`

**Solution**:
- Verify the build output path matches the artifact path
- Check that the build completed successfully before uploading
- Ensure the artifact names are unique for each platform

```yaml
- name: Upload artifacts (Windows)
  if: runner.os == 'Windows'
  uses: actions/upload-artifact@v3
  with:
    name: WorkDayTracker-Windows
    path: build/Release/WorkDayTracker.exe  # Verify this path exists
```

## Debugging Steps

### 1. Enable Verbose Logging

Add verbose output to your workflow to get more detailed information:

```yaml
- name: Configure CMake
  run: cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$QT_ROOT_DIR --debug-output
```

### 2. Check Environment Variables

Add a step to print environment variables to verify Qt installation:

```yaml
- name: Debug Qt Installation
  run: |
    echo "QT_ROOT_DIR: $QT_ROOT_DIR"
    echo "CMAKE_PREFIX_PATH: $CMAKE_PREFIX_PATH"
    ls -la $QT_ROOT_DIR
```

### 3. Manual Qt Installation (Alternative)

If the install-qt-action continues to fail, you can try manual installation:

```yaml
- name: Install Qt manually (Ubuntu)
  if: runner.os == 'Linux'
  run: |
    sudo apt-get update
    sudo apt-get install -y qt6-base-dev libqt6charts6-dev
```

## Platform-Specific Issues

### Windows

- Ensure you're using the correct Visual Studio version
- Check that the Windows SDK is available
- Verify the executable path (build/Release/ vs build/)

### Linux

- Install required dependencies:
  ```bash
  sudo apt-get install build-essential cmake
  ```

- Ensure the Qt libraries are correctly linked

### macOS

- Check Xcode command line tools installation
- Verify the macOS deployment target

## Updating the Workflow

If you need to update the workflow:

1. Check the latest versions of the actions used:
   - `actions/checkout@v3` → `actions/checkout@v4`
   - `jurplel/install-qt-action@v3` → latest version
   - `actions/upload-artifact@v3` → `actions/upload-artifact@v4`

2. Update the Qt version if needed:
   ```yaml
   qt_version: [6.10.0]  # Instead of 6.9.2
   ```

3. Add matrix testing for multiple Qt versions:
   ```yaml
   strategy:
     matrix:
       os: [ubuntu-latest, windows-latest, macos-latest]
       qt_version: [6.9.2, 6.10.0]
   ```

## Reporting Issues

If you continue to experience issues:

1. Check the GitHub Actions logs for specific error messages
2. Verify that your changes didn't introduce any breaking changes
3. Test the build process locally first
4. Create an issue with:
   - The exact error message
   - The workflow file you're using
   - Your operating system and Qt version
   - Any recent changes to the codebase