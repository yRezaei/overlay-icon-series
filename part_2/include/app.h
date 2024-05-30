#ifndef C02C8E9A_E3D6_4971_91B6_BD1AA135B225
#define C02C8E9A_E3D6_4971_91B6_BD1AA135B225

#include "util/win32_util.h"
#include "util/image_util.h"
#include "gfx/image.h"
#include "gfx/color.h"
#include "gfx/font.h"
#include "geom/size.h"
#include "ui/button.h"
#include "ui/image_widget.h"
#include <memory>

class App {
public:
    App(HINSTANCE hInstance);
    bool initialize();
    void run();

private:
    HINSTANCE hInstance;
    HWND hwnd;
    int overlay_number;
    std::shared_ptr<gfx::Font> font_;
    util::win32::AppData appData;
    gfx::Image app_img_;

    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool registerWindowClass(const wchar_t* className);
    void setupUI();

    void incrementOverlayNumber(ui::ImageWidget* imageWidget);
    void decrementOverlayNumber(ui::ImageWidget* imageWidget);
    void updateOverlayIcon(ui::ImageWidget* imageWidget);
    void showWarningIcon(ui::ImageWidget* imageWidget);
    void showErrorIcon(ui::ImageWidget* imageWidget);
    void showRedCircle(ui::ImageWidget* imageWidget);
};

#endif /* C02C8E9A_E3D6_4971_91B6_BD1AA135B225 */
