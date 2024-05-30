#ifndef F7F59270_F63B_4B79_9110_FEDF3C67AAAE
#define F7F59270_F63B_4B79_9110_FEDF3C67AAAE

#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <commctrl.h>
#include <shellapi.h>
#include <Shobjidl.h>
#include <gdiplus.h>
#include "gfx/image.h"

using namespace Gdiplus;

namespace ui
{
    class Widget;
}

namespace util
{
    namespace win32
    {
        struct AppData
        {
            HINSTANCE h_instance;
            HICON h_app_icon;
            int counter;
            ULONG_PTR gdiplus_token;
            std::vector<ui::Widget*> widgets;
        };

        /// Initialize window with the provided icon
        void initialize_window(HWND hwnd, HICON h_icon);
        /// Initialize tray icon with the provided icon
        void initialize_tray_icon(HWND hwnd, HICON h_icon);
        /// Set the overlay icon with the given count
        void set_overlay_icon(HWND hwnd, int count);
        /// Create an icon from the given file path
        HICON create_icon_from_file(const std::wstring &file_path);
        /// Create a bitmap from the given number
        HBITMAP create_bitmap_from_number(int number);
        /// Create an icon from the given bitmap
        HICON create_icon_from_bitmap(HBITMAP h_bitmap);
        /// Create the application window
        HWND create_app_window(HINSTANCE h_instance, const wchar_t *class_name);
        /// Initialize GDI+
        bool initialize_gdiplus(ULONG_PTR *gdiplus_token);
        /// Shutdown GDI+
        void shutdown_gdiplus(ULONG_PTR gdiplus_token);
        /// Show an error message box
        void show_error_message(const wchar_t *message);

        inline HWND create_app_window(HINSTANCE h_instance, const wchar_t *class_name)
        {
            return CreateWindowEx(
                WS_EX_CLIENTEDGE,
                class_name,
                L"Sample Window",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                NULL, NULL, h_instance, NULL);
        }

        inline bool initialize_gdiplus(ULONG_PTR *gdiplus_token)
        {
            Gdiplus::GdiplusStartupInput gdiplus_startup_input;
            return Gdiplus::GdiplusStartup(gdiplus_token, &gdiplus_startup_input, nullptr) == Gdiplus::Ok;
        }

        inline void shutdown_gdiplus(ULONG_PTR gdiplus_token)
        {
            Gdiplus::GdiplusShutdown(gdiplus_token);
        }

        inline void show_error_message(const wchar_t *message)
        {
            MessageBox(NULL, message, TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        }

        inline void initialize_window(HWND hwnd, HICON h_icon)
        {
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)h_icon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)h_icon);
        }

        inline void create_icon_data(HWND hwnd, HICON h_icon, NOTIFYICONDATA& nid) {
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_USER + 1;
            nid.hIcon = h_icon;
            wcscpy_s(nid.szTip, L"Sample Tray Icon");
        }

        inline void initialize_tray_icon(HWND hwnd, HICON h_icon)
        {
            NOTIFYICONDATA nid = {};
            create_icon_data(hwnd, h_icon, nid);
            Shell_NotifyIcon(NIM_ADD, &nid);
        }

        inline void update_tray_icon(HWND hwnd, HICON h_icon)
        {
            NOTIFYICONDATA nid = {};
            create_icon_data(hwnd, h_icon, nid);
            Shell_NotifyIcon(NIM_MODIFY, &nid);
        }

        inline void set_overlay_icon(HWND hwnd, int count)
        {
            HBITMAP h_bitmap = create_bitmap_from_number(count);
            HICON h_overlay_icon = count == 0 ? nullptr : create_icon_from_bitmap(h_bitmap);

            HRESULT hr = SetCurrentProcessExplicitAppUserModelID(L"MyCompany.MyApp");
            if (SUCCEEDED(hr))
            {
                ITaskbarList3 *p_taskbar_list;
                hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (void **)&p_taskbar_list);
                if (SUCCEEDED(hr))
                {
                    if (h_overlay_icon)
                    {
                        hr = p_taskbar_list->SetOverlayIcon(hwnd, h_overlay_icon, L"Overlay Icon");
                        if (SUCCEEDED(hr))
                        {
                            p_taskbar_list->Release();
                            DeleteObject(h_bitmap);
                            DestroyIcon(h_overlay_icon);
                            return;
                        }
                    }
                    else
                    {
                        hr = p_taskbar_list->SetOverlayIcon(hwnd, NULL, NULL);
                        if (SUCCEEDED(hr))
                        {
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

        inline HICON create_icon_from_file(const std::wstring &file_path)
        {
            Gdiplus::Bitmap *bitmap = Gdiplus::Bitmap::FromFile(file_path.c_str());
            HICON h_icon = NULL;
            if (bitmap)
            {
                bitmap->GetHICON(&h_icon);
                delete bitmap;
            }
            return h_icon;
        }

        inline HBITMAP create_bitmap_from_number(int number)
        {
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

        inline HICON create_icon_from_bitmap(HBITMAP h_bitmap)
        {
            ICONINFO ii = {0};
            ii.fIcon = TRUE;
            ii.hbmMask = h_bitmap;
            ii.hbmColor = h_bitmap;
            return CreateIconIndirect(&ii);
        }

        inline HICON create_icon_from_image(const gfx::Image &image)
        {
            if (!image.is_valid() || image.channels < 3)
            {
                return nullptr;
            }

            int width = image.size.width;
            int height = image.size.height;

            // Create a DIB (Device Independent Bitmap) section
            BITMAPV5HEADER bi;
            memset(&bi, 0, sizeof(bi));
            bi.bV5Size = sizeof(bi);
            bi.bV5Width = width;
            bi.bV5Height = -height; // Negative to indicate top-down DIB
            bi.bV5Planes = 1;
            bi.bV5BitCount = 32;
            bi.bV5Compression = BI_BITFIELDS;
            bi.bV5RedMask = 0x00FF0000;
            bi.bV5GreenMask = 0x0000FF00;
            bi.bV5BlueMask = 0x000000FF;
            bi.bV5AlphaMask = 0xFF000000;

            HDC hdc = GetDC(NULL);
            void *bits = nullptr;
            HBITMAP hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, &bits, NULL, 0);
            ReleaseDC(NULL, hdc);

            if (!hBitmap)
            {
                return nullptr;
            }

            // Copy the image data into the DIB section
            uint8_t *dest = (uint8_t *)bits;
            const uint8_t *src = image.buffer;

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    int srcIndex = (y * width + x) * image.channels;
                    int destIndex = (y * width + x) * 4;

                    dest[destIndex] = src[srcIndex + 2]; // Red
                    dest[destIndex + 1] = src[srcIndex + 1]; // Green
                    dest[destIndex + 2] = src[srcIndex];         // Blue
                    dest[destIndex + 3] = (image.channels == 4) ? src[srcIndex + 3] : 0xFF; // Alpha
                }
            }

            // Create the icon
            ICONINFO iconInfo = {};
            iconInfo.fIcon = TRUE;
            iconInfo.xHotspot = 0;
            iconInfo.yHotspot = 0;
            iconInfo.hbmMask = hBitmap;
            iconInfo.hbmColor = hBitmap;

            HICON hIcon = CreateIconIndirect(&iconInfo);

            // Clean up
            DeleteObject(hBitmap);

            return hIcon;
        }
    }
}

#endif /* F7F59270_F63B_4B79_9110_FEDF3C67AAAE */
