#ifndef E90E15D0_1DC3_4EA4_998E_C3C98382F6F1
#define E90E15D0_1DC3_4EA4_998E_C3C98382F6F1

#include <algorithm>

namespace geom
{

    class Rect
    {
    public:
        int x, y, right, bottom;
        Rect(int x = 0, int y = 0, int right = 0, int bottom = 0)
            : x(x), y(y), right(right), bottom(bottom) {}

        int width() const
        {
            return right - x;
        }

        int height() const
        {
            return bottom - y;
        }

        void moveTo(int newX, int newY)
        {
            int w = width();
            int h = height();
            x = newX;
            y = newY;
            right = x + w;
            bottom = y + h;
        }

        void resize(int newWidth, int newHeight)
        {
            right = x + newWidth;
            bottom = y + newHeight;
        }

        bool contains(const Rect &other) const
        {
            return x <= other.x && y <= other.y &&
                   right >= other.right && bottom >= other.bottom;
        }

        Rect intersection(const Rect &other) const
        {
            int newX = std::max(x, other.x);
            int newY = std::max(y, other.y);
            int newRight = std::min(right, other.right);
            int newBottom = std::min(bottom, other.bottom);
            if (newRight < newX || newBottom < newY)
            {
                return Rect(0, 0, 0, 0);
            }
            return Rect(newX, newY, newRight, newBottom);
        }
    };
}

#endif /* E90E15D0_1DC3_4EA4_998E_C3C98382F6F1 */
