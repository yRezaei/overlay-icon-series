#ifndef F542BE83_F46F_45AE_8A1F_85B3D447F920
#define F542BE83_F46F_45AE_8A1F_85B3D447F920

#include <algorithm>

namespace geom
{

    struct Size
    {
        int width;
        int height;

        // Default constructor
        Size() : width(0), height(0) {}

        // Parameterized constructor
        Size(int w, int h) : width(w), height(h) {}

        // Copy constructor
        Size(const Size &other) : width(other.width), height(other.height) {}

        // Move constructor
        Size(Size &&other) noexcept : width(other.width), height(other.height)
        {
            other.width = 0;
            other.height = 0;
        }

        // Copy assignment operator
        Size &operator=(const Size &other)
        {
            if (this != &other)
            {
                width = other.width;
                height = other.height;
            }
            return *this;
        }

        // Move assignment operator
        Size &operator=(Size &&other) noexcept
        {
            if (this != &other)
            {
                width = other.width;
                height = other.height;

                other.width = 0;
                other.height = 0;
            }
            return *this;
        }
    };
}

#endif /* F542BE83_F46F_45AE_8A1F_85B3D447F920 */
