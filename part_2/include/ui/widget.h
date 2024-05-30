#ifndef B56073ED_CB72_485D_B5F7_E5C0B3DE6892
#define B56073ED_CB72_485D_B5F7_E5C0B3DE6892

#include "ui/events.h"
#include "geom/rect.h"
#include <windows.h>

namespace ui
{
    class Widget
    {
    public:
        Widget(HWND hwnd) : hwnd(hwnd), fixedSize(false), minSize(0, 0, 0, 0), maxSize(0, 0, 0, 0) {}
        virtual ~Widget() = default;

        // Event handlers
        virtual void onMouseEvent(const MouseInfo &mouseInfo) {};
        virtual void onResizeEvent(const geom::Rect &rect) {};
        virtual void onPaintEvent(HDC hdc) = 0;

        void setAnchor(const geom::Point &anchor)
        {
            this->anchor = anchor;
        }

        geom::Point getAnchor() const
        {
            return anchor;
        }

        void setFixedSize(bool fixedSize)
        {
            this->fixedSize = fixedSize;
        }

        bool isFixedSize() const
        {
            return fixedSize;
        }

        void setMinSize(const geom::Rect &minSize)
        {
            this->minSize = minSize;
        }

        void setMaxSize(const geom::Rect &maxSize)
        {
            this->maxSize = maxSize;
        }

        void setRect(const geom::Rect &rect)
        {
            this->rect = rect;
        }

        geom::Rect getRect() const
        {
            return rect;
        }

        void moveTo(int x, int y)
        {
            rect.moveTo(x, y);
        }

        void resize(int width, int height)
        {
            if (!fixedSize)
            {
                if (width < minSize.width())
                    width = minSize.width();
                if (height < minSize.height())
                    height = minSize.height();
                if (maxSize.width() > 0 && width > maxSize.width())
                    width = maxSize.width();
                if (maxSize.height() > 0 && height > maxSize.height())
                    height = maxSize.height();
                rect.resize(width, height);
            }
        }

        void invalidate()
        {
            RECT r{rect.x, rect.y, rect.right, rect.bottom};
            InvalidateRect(hwnd, &r, TRUE);
        }

        bool isInside(const geom::Point &pos) const
        {
            return pos.x >= rect.x && pos.x <= rect.right && pos.y >= rect.y && pos.y <= rect.bottom;
        }

    protected:
        HWND hwnd;
        geom::Rect rect;
        geom::Point anchor;
        bool fixedSize;
        geom::Rect minSize;
        geom::Rect maxSize;
    };
}

#endif /* B56073ED_CB72_485D_B5F7_E5C0B3DE6892 */
