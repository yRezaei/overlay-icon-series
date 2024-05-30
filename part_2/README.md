# Overlay Icon Series - Part 2

## Purpose

This project is part of the "Overlay Icon Series" and serves as a practical example for the article ["The Importance of Overlay Icons: Part 2"](https://www.linkedin.com/pulse/importance-overlay-icons-part-1-yashar-abbasalizadeh-rezaei-hqrle/?trackingId=iJKD%2BfkhQ7%2BVNBd6m9dXhg%3D%3D). The main goal is to demonstrate how to enhance application communication using overlay icons in various parts of a Windows application, including the window, taskbar, and tray icon.

## Project Overview

This application creates a window with five button widgets and an image widget. When the buttons are clicked, an overlay image is added to the main application icon (`app_icon.png`). The updated image is then set as the icon for the window, taskbar, and tray icon of the application.

### Important Functionalities

1. **Loading and Creating Icons**: Functions to load images from files and create icons.
2. **Overlaying Images**: Functionality to overlay one image on top of another.
3. **Setting Icons**: Functions to set the icons for the window, taskbar, and tray icon.
4. **Interactive UI**: A simple user interface with buttons to demonstrate the dynamic updating of icons.

### Key Functions

- **`create_icon_from_file`**: Reads an image file and converts it into an `HICON`.
- **`overlay_image`**: Combines two images with specified positions and ratios to create an overlay effect.
- **`initialize_window` and `initialize_tray_icon`**: Initialize and update the window and tray icons.
- **`render_text`**: Renders text onto an image using the STB library.
- **`draw_circle`**: Draws a circle onto an image.

## Requirements

- CMake 3.10 or higher
- A C++17 compatible compiler
- Windows OS
- Libraries: `user32`, `gdi32`, `shell32`, `gdiplus`, `comctl32`, `Msimg32`

## Project Structure

```plaintext
part 2
├── files
├── include
│   ├── geom
│   │   ├── point.h
│   │   ├── rect.h
│   │   └── size.h
│   ├── gfx
│   │   ├── color.h
│   │   ├── font.h
│   │   └── image.h
│   ├── ui
│   │   ├── button.h
│   │   ├── events.h
│   │   ├── image_widget.h
│   │   └── widget.h
│   └── util
│       ├── image_util.h
│       └── win32_util.h
│   └── app.h
├── src
│   ├── app.cpp
│   ├── image_util.cpp
│   ├── main.cpp
│   └── stb_impl.cpp
└── CMakeLists.txt
```
## How to Compile and Build
### Clone the repository:
```bash
git clone https://github.com/yRezaei/overlay-icon-series.git
cd overlay-icon-series/part_2
```

### Generate build files using CMake:
```bash
mkdir build
cd build
cmake ..
```

### Build the project:
```bash
cmake --build .
```

### Run the executable:
```bash
./bin/Debug/overlay_icon_p2.exe # or the appropriate path based on your build configuration
```

By following these steps, you will be able to build and run the application, and explore the dynamic overlay icon functionality demonstrated in this part of the series.

For more details, please refer to the accompanying article: ["The Importance of Overlay Icons: Part 2"](https://www.linkedin.com/pulse/importance-overlay-icons-part-2-yashar-abbasalizadeh-rezaei-beoff).