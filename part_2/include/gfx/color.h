#ifndef DEE436EF_BC93_45BB_8383_39F54A2A9E8B
#define DEE436EF_BC93_45BB_8383_39F54A2A9E8B

#include <cstdint>

namespace gfx
{

    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        // Default constructor
        Color() : r(0), g(0), b(0), a(255) {}

        // Parameterized constructor
        Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}

        // Copy constructor
        Color(const Color &other)
            : r(other.r), g(other.g), b(other.b), a(other.a) {}

        // Move constructor
        Color(Color &&other) noexcept
            : r(other.r), g(other.g), b(other.b), a(other.a)
        {
            other.r = other.g = other.b = other.a = 0;
        }

        // Copy assignment operator
        Color &operator=(const Color &other)
        {
            if (this != &other)
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
            }
            return *this;
        }

        // Move assignment operator
        Color &operator=(Color &&other) noexcept
        {
            if (this != &other)
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
                other.r = other.g = other.b = other.a = 0;
            }
            return *this;
        }

        // Static functions for predefined colors
        static Color light_red() { return Color(255, 102, 102); }
        static Color normal_red() { return Color(255, 0, 0); }
        static Color dark_red() { return Color(153, 0, 0); }

        static Color light_green() { return Color(102, 255, 102); }
        static Color normal_green() { return Color(0, 255, 0); }
        static Color dark_green() { return Color(0, 153, 0); }

        static Color light_blue() { return Color(102, 102, 255); }
        static Color normal_blue() { return Color(0, 0, 255); }
        static Color dark_blue() { return Color(0, 0, 153); }

        static Color light_yellow() { return Color(255, 255, 102); }
        static Color normal_yellow() { return Color(255, 255, 0); }
        static Color dark_yellow() { return Color(153, 153, 0); }

        static Color light_cyan() { return Color(102, 255, 255); }
        static Color normal_cyan() { return Color(0, 255, 255); }
        static Color dark_cyan() { return Color(0, 153, 153); }

        static Color light_magenta() { return Color(255, 102, 255); }
        static Color normal_magenta() { return Color(255, 0, 255); }
        static Color dark_magenta() { return Color(153, 0, 153); }

        static Color light_gray() { return Color(192, 192, 192); }
        static Color normal_gray() { return Color(128, 128, 128); }
        static Color dark_gray() { return Color(64, 64, 64); }

        static Color black() { return Color(0, 0, 0); }
        static Color white() { return Color(255, 255, 255); }
    };

} // namespace graphics

#endif /* DEE436EF_BC93_45BB_8383_39F54A2A9E8B */
