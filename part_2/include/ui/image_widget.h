#ifndef CAFD3C26_50F7_43E6_ABBB_EB86695D43BA
#define CAFD3C26_50F7_43E6_ABBB_EB86695D43BA

#include "ui/widget.h"
#include "gfx/image.h"
#include <string>

namespace ui
{

    class ImageWidget : public ui::Widget
    {
    public:
        ImageWidget(HWND hwnd, const gfx::Image &image)
            : Widget(hwnd), image(image)
        {
            adjustSize();
        }

        void setImage(const gfx::Image &new_image)
        {
            image = new_image;
            adjustSize();
            invalidate();
        }

        void onPaintEvent(HDC hdc) override
        {
            RECT r{rect.x, rect.y, rect.right, rect.bottom};
            FillRect(hdc, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

            HBITMAP hBitmap = createBitmapFromImage(image);
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hBitmap);

            // Use AlphaBlend for proper transparency handling
            BLENDFUNCTION blendFunc;
            blendFunc.BlendOp = AC_SRC_OVER;
            blendFunc.BlendFlags = 0;
            blendFunc.SourceConstantAlpha = 255;
            blendFunc.AlphaFormat = AC_SRC_ALPHA;

            AlphaBlend(hdc, rect.x, rect.y, rect.width(), rect.height(), hdcMem, 0, 0, image.size.width, image.size.height, blendFunc);

            DeleteDC(hdcMem);
            DeleteObject(hBitmap);
        }

    private:
        gfx::Image image;

        void adjustSize()
        {
            rect.right = rect.x + image.size.width;
            rect.bottom = rect.y + image.size.height;
        }

        HBITMAP createBitmapFromImage(const gfx::Image &image)
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

                    dest[destIndex] = src[srcIndex + 2];                                    // Red
                    dest[destIndex + 1] = src[srcIndex + 1];                                // Green
                    dest[destIndex + 2] = src[srcIndex];                                    // Blue
                    dest[destIndex + 3] = (image.channels == 4) ? src[srcIndex + 3] : 0xFF; // Alpha
                }
            }
            return hBitmap;
        }
    };
}

#endif /* CAFD3C26_50F7_43E6_ABBB_EB86695D43BA */
