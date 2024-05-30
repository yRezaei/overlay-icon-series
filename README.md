# Overlay Icon Series

Welcome to the Overlay Icon Series repository! This series explores the utility and implementation of overlay icons in Windows applications. Each part of the series focuses on different aspects of overlay icons, from their importance to practical implementation.

## Overview

Overlay icons are small, dynamic icons that appear over an application's taskbar button. They provide visual indicators to convey status or notifications to the user, even when the application is minimized or running in the background. This series will help you understand the significance of overlay icons and how to implement them in your applications.

## Parts of the Series

### Part 1: The Importance of Overlay Icons in the Taskbar

In the first part, we discuss why overlay icons are important and how they can enhance user interaction with your application. We also cover the basics of implementing overlay icons in Windows using C++.

- [Read the Article](https://www.linkedin.com/pulse/importance-overlay-icons-part-1-yashar-abbasalizadeh-rezaei-hqrle/)
- [View the Code](https://github.com/yRezaei/overlay-icon-series/tree/main/part_1)

### Part 2: Enhancing Application Communication with Overlay Icons
In the second part of the series, we take the use of overlay icons further by demonstrating how to dynamically update icons within a Windows application. This part covers the implementation of overlay icons for the window, taskbar, and tray icon using the STB library for efficient image processing. By following this guide, you'll learn to create responsive and visually informative interfaces that significantly improve user interaction.

- [Read the Article](https://www.linkedin.com/pulse/importance-overlay-icons-part-2-yashar-abbasalizadeh-rezaei-beoff/)
- [View the Code](https://github.com/yRezaei/overlay-icon-series/tree/main/part_2)

## Prerequisites

- **CMake**: Version 3.10 or higher
- **C++ Compiler**: Supporting C++17 standard
- **Libraries**: Ensure the following libraries are available on your system:
  - `user32`
  - `gdi32`
  - `shell32`
  - `gdiplus`
  - `comctl32`
  - `Msimg32`
  
### Clone the Repository

```bash
git clone https://github.com/yRezaei/overlay-icon-series.git
cd overlay-icon-series
```
Create a Build Directory
```bash
mkdir build
cd build
```
#### Generate Build Files with CMake
```bash
cmake ..
```
#### Build a Specific Part
To build and run a specific part, you can use the --target option with cmake --build:

```bash
cmake --build . --target overlay_icon_p1
```
And similarly for part 2 and so on:

```bash
cmake --build . --target overlay_icon_p2
```
#### Running the Application
Navigate to the Output Directory
```bash
cd bin/Release   # or bin/Debug based on your build configuration
```
Run the Executable
```bash
./overlay_icon_p1.exe
```
Ensure that the files directory, which contains the app_icon.png, is correctly copied to the output directory. The application should start, and you can observe the overlay icon functionality in the taskbar.

## Contributing
Contributions are welcome! If you have suggestions or improvements, feel free to open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Stay Tuned
Stay tuned for upcoming parts of this series where we will delve deeper into more advanced techniques and use cases for overlay icons, exploring how they can be leveraged to further enrich your application's interaction with its users.

For the latest updates and discussions, follow me on [LinkedIn](https://www.linkedin.com/in/yashar-abbasalizadeh-rezaei-270100139/).