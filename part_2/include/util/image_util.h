#ifndef A5890A5D_A10F_4276_8277_5CFCA8652B21
#define A5890A5D_A10F_4276_8277_5CFCA8652B21

#include <string>
#include <utility>

namespace gfx {
    struct Image;
    struct Font;
    struct Color;    
}

namespace geom {
    struct Size;
    struct Point;
}

namespace util
{
    namespace img
    {
        enum class OverlayPosition
        {
            Left,
            Top,
            Right,
            Bottom,
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight
        };

        gfx::Image read_image(const std::string &path, int request_channels);

        void render_text(gfx::Image &img, const char *text, const gfx::Font &font, const gfx::Color &color);

        gfx::Image resize_overlay(const gfx::Image &overlay, const geom::Size &new_size);

        geom::Size calculate_new_size(const geom::Size &base_size, const geom::Size &overlay_size, float ratio);

        std::pair<int, int> calculate_overlay_position(const geom::Size &base_size, const geom::Size &overlay_size, OverlayPosition position);

        gfx::Image overlay_image(const gfx::Image &base, const gfx::Image &overlay, float ratio, OverlayPosition position);

        void save_image(const gfx::Image &img, const char *file_path);

        bool draw_circle(gfx::Image &image, int radius, const geom::Point &center, const gfx::Color &color);

    } // namespace img
} // namespace util

#endif /* A5890A5D_A10F_4276_8277_5CFCA8652B21 */
