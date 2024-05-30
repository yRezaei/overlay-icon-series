#ifndef D8D41DFF_1077_44EA_856A_962ED70C171D
#define D8D41DFF_1077_44EA_856A_962ED70C171D

#include "ui/widget.h"
#include "ui/events.h"
#include <chrono>
#include <algorithm>
#include <functional>
#include <string>

namespace ui
{

    enum class ButtonAlignment
    {
        IMAGE_LEFT_TEXT_RIGHT,
        TEXT_LEFT_IMAGE_RIGHT
    };

    class Button : public Widget
    {
    public:
        // Constructor for text only
        Button(HWND hwnd, const std::wstring &text)
            : Widget(hwnd), text(text), hovered(false), pressed(false), onClick(nullptr),
              alignment(ButtonAlignment::IMAGE_LEFT_TEXT_RIGHT), padding(10), spacing(5), image(NULL),
              textColor(RGB(255, 255, 255)), backgroundColor(RGB(86, 112, 245)), hoverColor(RGB(56, 87, 245)), borderColor(RGB(3, 27, 150))
        {
            adjustSize();
        }

        // Constructor for image only
        Button(HWND hwnd, HBITMAP image)
            : Widget(hwnd), hovered(false), pressed(false), onClick(nullptr),
              alignment(ButtonAlignment::IMAGE_LEFT_TEXT_RIGHT), padding(10), spacing(5), image(image),
              textColor(RGB(255, 255, 255)), backgroundColor(RGB(86, 112, 245)), hoverColor(RGB(56, 87, 245)), borderColor(RGB(3, 27, 150))
        {
            adjustSize();
        }

        // Constructor for both text and image
        Button(HWND hwnd, const std::wstring &text, HBITMAP image)
            : Widget(hwnd), text(text), hovered(false), pressed(false), onClick(nullptr),
              alignment(ButtonAlignment::IMAGE_LEFT_TEXT_RIGHT), padding(10), spacing(5), image(image),
              textColor(RGB(255, 255, 255)), backgroundColor(RGB(86, 112, 245)), hoverColor(RGB(56, 87, 245)), borderColor(RGB(3, 27, 150))
        {
            adjustSize();
        }

        void setOnClick(std::function<void()> callback)
        {
            onClick = callback;
        }

        void setText(const std::wstring &text)
        {
            this->text = text;
            adjustSize();
            invalidate();
        }

        void setImage(HBITMAP image)
        {
            this->image = image;
            adjustSize();
            invalidate();
        }

        void setPadding(int padding)
        {
            this->padding = padding;
            adjustSize();
            invalidate();
        }

        void setSpacing(int spacing)
        {
            this->spacing = spacing;
            adjustSize();
            invalidate();
        }

        void setAlignment(ButtonAlignment alignment)
        {
            this->alignment = alignment;
            adjustSize();
            invalidate();
        }

        void setTextColor(COLORREF color)
        {
            textColor = color;
            invalidate();
        }

        void setBackgroundColor(COLORREF color)
        {
            backgroundColor = color;
            invalidate();
        }

        void setHoverColor(COLORREF color)
        {
            hoverColor = color;
            invalidate();
        }

        void setBorderColor(COLORREF color)
        {
            borderColor = color;
            invalidate();
        }

        void onMouseEvent(const MouseInfo &mouseInfo) override
        {
            bool inside = isInside(mouseInfo.pos);
            bool stateChanged = false;

            switch (mouseInfo.ev)
            {
            case MouseEvent::MOVED:
                if (inside && !hovered)
                {
                    hovered = true;
                    stateChanged = true;
                }
                else if (!inside && hovered)
                {
                    hovered = false;
                    stateChanged = true;
                }
                break;

            case MouseEvent::PRESSED:
                if (inside && mouseInfo.btn == MouseButton::LEFT)
                {
                    pressed = true;
                    pressTime = std::chrono::steady_clock::now();
                    stateChanged = true;
                }
                break;

            case MouseEvent::RELEASED:
                if (pressed && inside && mouseInfo.btn == MouseButton::LEFT)
                {
                    auto releaseTime = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed = releaseTime - pressTime;
                    if (elapsed.count() <= 0.3 && onClick)
                    {
                        onClick();
                    }
                }
                pressed = false;
                stateChanged = true;
                break;

            default:
                break;
            }

            if (stateChanged)
            {
                invalidate();
            }
        }

        void onResizeEvent(const geom::Rect &rect) override
        {
            invalidate();
        }

        void onPaintEvent(HDC hdc) override
        {
            COLORREF bgColor = hovered ? hoverColor : backgroundColor;
            HBRUSH brush = CreateSolidBrush(bgColor);
            RECT r{rect.x, rect.y, rect.right, rect.bottom};
            FillRect(hdc, &r, brush);
            DeleteObject(brush);

            int x = rect.x + padding;
            int y = rect.y + padding;

            SetBkMode(hdc, TRANSPARENT); // Make the text background transparent
            SetTextColor(hdc, textColor); // Set the text color

            if (!text.empty() || image != NULL)
            {
                if (alignment == ButtonAlignment::TEXT_LEFT_IMAGE_RIGHT)
                {
                    if (!text.empty())
                    {
                        x += drawText(hdc, x, y);
                        if (image != NULL)
                        {
                            x += spacing;
                            drawImage(hdc, x, y);
                        }
                    }
                }
                else
                {
                    if (image != NULL)
                    {
                        x += drawImage(hdc, x, y);
                        if (!text.empty())
                        {
                            x += spacing;
                            drawText(hdc, x, y);
                        }
                    }
                    else if (!text.empty())
                    {
                        drawText(hdc, x, y);
                    }
                }
            }

            if (pressed)
            {
                HBRUSH borderBrush = CreateSolidBrush(borderColor);
                FrameRect(hdc, &r, borderBrush);
                DeleteObject(borderBrush);
            }
        }

    private:
        bool hovered;
        bool pressed;
        std::chrono::steady_clock::time_point pressTime;
        std::function<void()> onClick;

        std::wstring text;
        HBITMAP image;
        ButtonAlignment alignment;
        int padding;
        int spacing;
        COLORREF textColor;
        COLORREF backgroundColor;
        COLORREF hoverColor;
        COLORREF borderColor;

        void adjustSize()
        {
            int width = padding * 4;  // Left and right padding
            int height = padding * 2; // Top and bottom padding

            HDC hdc = GetDC(hwnd);
            if (!text.empty())
            {
                SIZE textSize;
                HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
                SelectObject(hdc, hFont);
                GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &textSize);
                width += textSize.cx;
                height += textSize.cy;
                DeleteObject(hFont);
            }
            if (image != NULL)
            {
                BITMAP bitmap;
                GetObject(image, sizeof(BITMAP), &bitmap);
                width += bitmap.bmWidth + (text.empty() ? 0 : spacing);         // Add spacing if text is also present
                height = std::max((LONG)height, bitmap.bmHeight + padding * 2); // Ensure height is enough for the image
            }
            ReleaseDC(hwnd, hdc);

            rect.right = rect.x + std::max(width, 100);  // Ensure minimum width is 100
            rect.bottom = rect.y + std::max(height, 30); // Ensure minimum height is 30
        }

        int drawText(HDC hdc, int x, int y) const
        {
            RECT textRect = {x, y, rect.right - padding, rect.bottom - padding};
            DrawText(hdc, text.c_str(), -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            SIZE textSize;
            GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &textSize);
            return textSize.cx;
        }

        int drawImage(HDC hdc, int x, int y) const
        {
            BITMAP bitmap;
            GetObject(image, sizeof(BITMAP), &bitmap);
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, image);
            BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);
            return bitmap.bmWidth;
        }
    };
}

#endif /* D8D41DFF_1077_44EA_856A_962ED70C171D */
