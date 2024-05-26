
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

# Part 1: The Importance of Overlay Icons in the Taskbar
## Description
The code in Part 1 demonstrates how to implement overlay icons in the Windows taskbar using C++. Overlay icons are small, dynamic icons that appear over an application's taskbar button, providing visual indicators to convey status or notifications to the user.

This application includes functionalities to:

Initialize and set up the main window and tray icon.
Create icons from files and dynamically generated bitmaps.
Update the overlay icon based on user interaction.

## Usage
The application allows users to interact with it using left and right mouse clicks to increment or decrement a counter, respectively. The counter value is displayed as an overlay icon in the taskbar.

- Left Click: Increases the counter and updates the overlay icon.
- Right Click: Decreases the counter and updates the overlay icon.

## How to Compile and Run
### Clone the Repository

```bash
git clone https://github.com/yRezaei/overlay-icon-series.git
cd overlay-icon-series
```

### Create a Build Directory
```bash
mkdir build
cd build
```

### Generate Build Files with CMake
```bash
cmake ..
```

### Build the Project
```bash
cmake --build . --target overlay_icon_p1
```

### Navigate to the Output Directory
```bash
cd bin/Release   # or bin/Debug based on your build configuration
```

### Run the Executable
```bash
./overlay_icon_p1.exe
```

Ensure that the files directory, which contains the app_icon.png, is correctly copied to the output directory. The application should start, and you can observe the overlay icon functionality in the taskbar.

## Code Overview
### Main Window Procedure
The window_proc function handles the main window messages. It processes left and right mouse button clicks to update the overlay icon counter.

```cpp

LRESULT CALLBACK window_proc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) {
    AppData* app_data = (AppData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (u_msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_LBUTTONDOWN:
            app_data->counter++;
            set_overlay_icon(hwnd, app_data->counter);
            return 0;

        case WM_RBUTTONDOWN:
            --app_data->counter;
            if (app_data->counter < 0) 
                app_data->counter = 0;
            set_overlay_icon(hwnd, app_data->counter);
            return 0;

        default:
            return DefWindowProc(hwnd, u_msg, w_param, l_param);
    }
}
```

### Setting Overlay Icons
The set_overlay_icon function creates a bitmap from a number and sets it as the overlay icon on the taskbar.

```cpp

void set_overlay_icon(HWND hwnd, int count) {
    HBITMAP h_bitmap = create_bitmap_from_number(count);
    HICON h_overlay_icon = count == 0 ? nullptr : create_icon_from_bitmap(h_bitmap);

    HRESULT hr = SetCurrentProcessExplicitAppUserModelID(L"MyCompany.MyApp");
    if (SUCCEEDED(hr)) {
        ITaskbarList3 *p_taskbar_list;
        hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (void **)&p_taskbar_list);
        if (SUCCEEDED(hr)) {
            if (h_overlay_icon) {
                hr = p_taskbar_list->SetOverlayIcon(hwnd, h_overlay_icon, L"Overlay Icon");
                if (SUCCEEDED(hr)) {
                    p_taskbar_list->Release();
                    DeleteObject(h_bitmap);
                    DestroyIcon(h_overlay_icon);
                    return;
                }
            } else {
                hr = p_taskbar_list->SetOverlayIcon(hwnd, NULL, NULL);
                if (SUCCEEDED(hr)) {
                    p_taskbar_list->Release();
                    DeleteObject(h_bitmap);
                    DestroyIcon(h_overlay_icon);
                    return;
                }
            }
            p_taskbar_list->Release();
        }
    }
    DeleteObject(h_bitmap);
    DestroyIcon(h_overlay_icon);
}
```

### Creating Icons from Bitmaps
The create_bitmap_from_number function generates a bitmap with a given number rendered on it. This bitmap is then used to create an icon.

```cpp

HBITMAP create_bitmap_from_number(int number) {
    int size = 64;
    HDC hdc = GetDC(nullptr);
    HDC mem_dc = CreateCompatibleDC(hdc);
    HBITMAP h_bitmap = CreateCompatibleBitmap(hdc, size, size);
    SelectObject(mem_dc, h_bitmap);

    RECT rect = {0, 0, size, size};
    HBRUSH h_brush = CreateSolidBrush(RGB(200, 200, 200));
    FillRect(mem_dc, &rect, h_brush);
    DeleteObject(h_brush);

    std::wstringstream ws;
    ws << number;
    std::wstring str = ws.str();

    SetBkMode(mem_dc, TRANSPARENT);
    HFONT h_font = CreateFont((size / 3) * 2, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                             CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    SelectObject(mem_dc, h_font);

    DrawText(mem_dc, str.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    DeleteObject(h_font);
    DeleteDC(mem_dc);
    ReleaseDC(nullptr, hdc);

    return h_bitmap;
}

HICON create_icon_from_bitmap(HBITMAP h_bitmap) {
    ICONINFO ii = {0};
    ii.fIcon = TRUE;
    ii.hbmMask = h_bitmap;
    ii.hbmColor = h_bitmap;
    return CreateIconIndirect(&ii);
}
```

## Conclusion
By following the steps above, you can compile and run the Part 1 example, exploring the use of overlay icons to enhance user interaction. The application demonstrates the basic functionality of updating overlay icons based on user actions, providing a foundation for more complex implementations.

Stay tuned for the upcoming parts of this series where we will delve deeper into advanced techniques and use cases for overlay icons. If you have any thoughts or questions, feel free to leave a comment. See you in the next part of this series!