#include "app.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    App app(hInstance);

    if (!app.initialize()) {
        return -1;
    }

    app.run();
    return 0;
}