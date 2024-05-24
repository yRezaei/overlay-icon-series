
# Overlay Icon Project - Part 1

This README provides instructions on how to compile and run the overlay icon project using CMake.

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

## Project Structure
```
overlay_icon_project/
│
├── CMakeLists.txt
├── src/
│ └── main.cpp
└── files/
└── app_icon.png
```

## Compilation Instructions
Clone the Repository

```
git clone https://github.com/yRezaei/overlay-icon-series.git
cd overlay-icon-series/part_1
```

Create a Build Directory
```
mkdir build
cd build
```

Generate Build Files with CMake
```
cmake ..
```

This command will generate the necessary build files in the build directory.

Build the Project
```
cmake --build .
```
This command will compile the project and place the executable in the bin directory corresponding to the build type (e.g., Debug, Release).

Running the Application
Navigate to the Output Directory
```
cd bin/Release   # or bin/Debug based on your build configuration
```

Run the Executable
```
./overlay_icon_p1.exe
```

Ensure that the files directory, which contains the app_icon.png, is correctly copied to the output directory. The application should start and you can observe the overlay icon functionality in the taskbar.

## Conclusion
By following these steps, you will be able to compile and run the overlay icon project. This project demonstrates the importance of overlay icons in enhancing user interaction with applications. For further details and more examples, stay tuned for the next parts of this series.