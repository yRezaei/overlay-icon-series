#include <windows.h>
#include <string>
#include <sstream>
#include <commctrl.h>
#include <shellapi.h>
#include <Shobjidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

struct AppData {
    HINSTANCE h_instance;
    HICON h_app_icon;
    int counter;
    ULONG_PTR gdiplus_token;
};

/// Main window procedure
LRESULT CALLBACK window_proc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
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
/// Register the window class
bool register_window_class(HINSTANCE h_instance, const wchar_t* class_name);
/// Create the application window
HWND create_app_window(HINSTANCE h_instance, const wchar_t* class_name);
/// Initialize GDI+
bool initialize_gdiplus(ULONG_PTR* gdiplus_token);
/// Shutdown GDI+
void shutdown_gdiplus(ULONG_PTR gdiplus_token);
/// Show an error message box
void show_error_message(const wchar_t* message);

int APIENTRY wWinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPWSTR lp_cmd_line, int n_cmd_show) {
    AppData app_data = {};
    app_data.h_instance = h_instance;

    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    if (!initialize_gdiplus(&app_data.gdiplus_token)) {
        show_error_message(L"GDI+ Initialization Failed!");
        return -1;
    }

    if (!register_window_class(h_instance, CLASS_NAME)) {
        show_error_message(L"Window Registration Failed!");
        return -1;
    }

    HWND hwnd = create_app_window(h_instance, CLASS_NAME);
    if (hwnd == NULL) {
        show_error_message(L"Window Creation Failed!");
        shutdown_gdiplus(app_data.gdiplus_token);
        return -1;
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&app_data);

    ShowWindow(hwnd, n_cmd_show);
    UpdateWindow(hwnd);

    app_data.h_app_icon = create_icon_from_file(L"files/app_icon.png");
    initialize_window(hwnd, app_data.h_app_icon);
    initialize_tray_icon(hwnd, app_data.h_app_icon);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    shutdown_gdiplus(app_data.gdiplus_token);

    return 0;
}

bool register_window_class(HINSTANCE h_instance, const wchar_t* class_name) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = window_proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(AppData*);
    wc.hInstance = h_instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = class_name;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    return RegisterClassEx(&wc) != 0;
}

HWND create_app_window(HINSTANCE h_instance, const wchar_t* class_name) {
    return CreateWindowEx(
        WS_EX_CLIENTEDGE,
        class_name,
        L"Sample Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, h_instance, NULL);
}

bool initialize_gdiplus(ULONG_PTR* gdiplus_token) {
    Gdiplus::GdiplusStartupInput gdiplus_startup_input;
    return Gdiplus::GdiplusStartup(gdiplus_token, &gdiplus_startup_input, nullptr) == Gdiplus::Ok;
}

void shutdown_gdiplus(ULONG_PTR gdiplus_token) {
    Gdiplus::GdiplusShutdown(gdiplus_token);
}

void show_error_message(const wchar_t* message) {
    MessageBox(NULL, message, TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
}

void initialize_window(HWND hwnd, HICON h_icon) {
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)h_icon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)h_icon);
}

void initialize_tray_icon(HWND hwnd, HICON h_icon) {
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = h_icon;
    wcscpy_s(nid.szTip, L"Sample Tray Icon");

    Shell_NotifyIcon(NIM_ADD, &nid);
}

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

HICON create_icon_from_file(const std::wstring &file_path) {
    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(file_path.c_str());
    HICON h_icon = NULL;
    if (bitmap) {
        bitmap->GetHICON(&h_icon);
        delete bitmap;
    }
    return h_icon;
}

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
