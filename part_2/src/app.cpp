#include "app.h"

App::App(HINSTANCE hInstance)
    : hInstance(hInstance), 
    overlay_number(0), 
    font_(gfx::Font::load_from_file("files/arial.ttf", 32.0f)), 
    app_img_(util::img::read_image("files/app_icon.png", 4)) 
{
    appData.h_instance = hInstance;
}

bool App::initialize() {
    if (!util::win32::initialize_gdiplus(&appData.gdiplus_token)) {
        util::win32::show_error_message(L"GDI+ Initialization Failed!");
        return false;
    }

    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    if (!registerWindowClass(CLASS_NAME)) {
        util::win32::show_error_message(L"Window Registration Failed!");
        return false;
    }

    hwnd = util::win32::create_app_window(hInstance, CLASS_NAME);
    if (hwnd == NULL) {
        util::win32::show_error_message(L"Window Creation Failed!");
        util::win32::shutdown_gdiplus(appData.gdiplus_token);
        return false;
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&appData);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    appData.h_app_icon = util::win32::create_icon_from_file(L"files/app_icon.png");
    util::win32::initialize_window(hwnd, appData.h_app_icon);
    util::win32::initialize_tray_icon(hwnd, appData.h_app_icon);

    setupUI();
    InvalidateRect(hwnd, NULL, TRUE);

    return true;
}

void App::run() {
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    for (auto w : appData.widgets) {
        delete w;
    }

    util::win32::shutdown_gdiplus(appData.gdiplus_token);
}

LRESULT CALLBACK App::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    util::win32::AppData* appData = (util::win32::AppData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    ui::MouseInfo mouseInfo = { ui::MouseEvent::NONE, {0, 0}, ui::MouseButton::NONE };

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    {
        POINTS pts = MAKEPOINTS(lParam);
        mouseInfo.pos = { pts.x, pts.y };
        mouseInfo.ev = (uMsg == WM_MOUSEMOVE) ? ui::MouseEvent::MOVED
            : (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) ? ui::MouseEvent::PRESSED
            : (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP) ? ui::MouseEvent::RELEASED
            : (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK) ? ui::MouseEvent::DB_CLICK
            : ui::MouseEvent::NONE;

        mouseInfo.btn = (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDBLCLK) ? ui::MouseButton::LEFT
            : (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDBLCLK) ? ui::MouseButton::RIGHT
            : (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP || uMsg == WM_MBUTTONDBLCLK) ? ui::MouseButton::MIDDLE
            : ui::MouseButton::NONE;

        for (auto w : appData->widgets) {
             w->onMouseEvent(mouseInfo);
        }
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        for (auto w : appData->widgets) {
            w->onPaintEvent(hdc);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

bool App::registerWindowClass(const wchar_t* className) {
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(util::win32::AppData*);
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    return RegisterClassEx(&wc) != 0;
}

void App::setupUI() {
    auto imageWidget = new ui::ImageWidget(hwnd, app_img_);
    imageWidget->setRect(geom::Rect(300, 100, 350, 150));
    appData.widgets.push_back(imageWidget);

    auto increment_button = new ui::Button(hwnd, L"Increment +");
    increment_button->setRect(geom::Rect(100, 10, 230, 40));
    increment_button->setOnClick([this, imageWidget]() { incrementOverlayNumber(imageWidget); });
    appData.widgets.push_back(increment_button);

    auto decrement_button = new ui::Button(hwnd, L"Decrement -");
    decrement_button->setRect(geom::Rect(100, 60, 230, 90));
    decrement_button->setOnClick([this, imageWidget]() { decrementOverlayNumber(imageWidget); });
    appData.widgets.push_back(decrement_button);

    auto warning_button = new ui::Button(hwnd, L"Show Warning");
    warning_button->setRect(geom::Rect(100, 110, 230, 140));
    warning_button->setOnClick([this, imageWidget]() { showWarningIcon(imageWidget); });
    appData.widgets.push_back(warning_button);

    auto error_button = new ui::Button(hwnd, L"Show Error");
    error_button->setRect(geom::Rect(100, 160, 230, 190));
    error_button->setOnClick([this, imageWidget]() { showErrorIcon(imageWidget); });
    appData.widgets.push_back(error_button);

    auto red_circle_button = new ui::Button(hwnd, L"Show Red Circle");
    red_circle_button->setRect(geom::Rect(100, 210, 230, 240));
    red_circle_button->setOnClick([this, imageWidget]() { showRedCircle(imageWidget); });
    appData.widgets.push_back(red_circle_button);
}

void App::incrementOverlayNumber(ui::ImageWidget* imageWidget) {
    ++overlay_number;
    updateOverlayIcon(imageWidget);
}

void App::decrementOverlayNumber(ui::ImageWidget* imageWidget) {
    --overlay_number;
    if (overlay_number < 0) overlay_number = 0;
    updateOverlayIcon(imageWidget);
}

void App::updateOverlayIcon(ui::ImageWidget* imageWidget) {
    if (overlay_number > 0) {
        gfx::Image number_image = gfx::Image(geom::Size{ 32, 32 }, 4, gfx::Color{ 0, 122, 204 });
        util::img::render_text(number_image, std::to_string(overlay_number).c_str(), *font_, gfx::Color{ 255, 255, 255 });
        auto result = util::img::overlay_image(app_img_, number_image, 1.0f / 2.0f, util::img::OverlayPosition::TopRight);
        imageWidget->setImage(result);
        auto icon = util::win32::create_icon_from_image(result);
        util::win32::update_tray_icon(hwnd, icon);
        util::win32::initialize_window(hwnd, icon);
    }
    else {
        imageWidget->setImage(app_img_);
        auto icon = util::win32::create_icon_from_image(app_img_);
        util::win32::update_tray_icon(hwnd, icon);
        util::win32::initialize_window(hwnd, icon);
    }
}

void App::showWarningIcon(ui::ImageWidget* imageWidget) {
    gfx::Image warning_img = util::img::read_image("files/warning.png", 4);
    auto result = util::img::overlay_image(app_img_, warning_img, 1.0f / 2.0f, util::img::OverlayPosition::TopRight);
    imageWidget->setImage(result);
    auto icon = util::win32::create_icon_from_image(result);
    util::win32::update_tray_icon(hwnd, icon);
    util::win32::initialize_window(hwnd, icon);
}

void App::showErrorIcon(ui::ImageWidget* imageWidget) {
    gfx::Image error_img = util::img::read_image("files/error.png", 4);
    auto result = util::img::overlay_image(app_img_, error_img, 1.0f / 2.0f, util::img::OverlayPosition::TopRight);
    imageWidget->setImage(result);
    auto icon = util::win32::create_icon_from_image(result);
    util::win32::update_tray_icon(hwnd, icon);
    util::win32::initialize_window(hwnd, icon);
}

void App::showRedCircle(ui::ImageWidget* imageWidget) {
    gfx::Image red_circle = gfx::Image(geom::Size{ 32, 32 }, 4, gfx::Color{ 0, 0, 0, 0 });
    util::img::draw_circle(red_circle, 5, geom::Point{16, 16}, gfx::Color(255, 0, 0));
    auto result = util::img::overlay_image(app_img_, red_circle, 1.0f / 2.0f, util::img::OverlayPosition::BottomRight);
    imageWidget->setImage(result);
    auto icon = util::win32::create_icon_from_image(result);
    util::win32::update_tray_icon(hwnd, icon);
    util::win32::initialize_window(hwnd, icon);
}
