#include "util/image_util.h"
#include "gfx/color.h"
#include "gfx/image.h"
#include "gfx/font.h"
#include "geom/size.h"
#include "geom/point.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_truetype.h"
#include "stb/stb_image_resize2.h"
#include "util/image_util.h"

namespace util
{
    namespace img
    {
        gfx::Image read_image(const std::string &path, int request_channels)
        {
            gfx::Image img;
            img.buffer = stbi_load(path.c_str(), &img.size.width, &img.size.height, &img.channels, request_channels);
            if (img.buffer)
            {
                img.channels = request_channels;
            }
            return img;
        }

        void render_text(gfx::Image &img, const char *text, const gfx::Font &font, const gfx::Color &color)
        {
            int ascent, descent, lineGap;
            stbtt_GetFontVMetrics(&font.font_info, &ascent, &descent, &lineGap);
            float baseline = ascent * font.scale;

            geom::Size size = img.size;
            uint8_t *pixels = img.buffer;

            // Calculate text width
            int textWidth = 0;
            int x = 0;
            for (int i = 0; text[i]; i++)
            {
                int advanceWidth, leftSideBearing;
                stbtt_GetCodepointHMetrics(&font.font_info, text[i], &advanceWidth, &leftSideBearing);
                textWidth += advanceWidth;

                if (text[i + 1])
                    textWidth += stbtt_GetCodepointKernAdvance(&font.font_info, text[i], text[i + 1]);
            }
            textWidth = static_cast<int>(textWidth * font.scale);

            // Calculate centered position
            int startX = (size.width - textWidth) / 2;
            int startY = static_cast<int>((size.height - baseline) / 2 + baseline);

            // Render text
            x = startX;
            for (int i = 0; text[i]; i++)
            {
                int advance, lsb, x0, y0, x1, y1;
                float x_shift = static_cast<float>(x) - static_cast<float>(floor(x));
                stbtt_GetCodepointHMetrics(&font.font_info, text[i], &advance, &lsb);
                stbtt_GetCodepointBitmapBoxSubpixel(&font.font_info, text[i], font.scale, font.scale, x_shift, 0, &x0, &y0, &x1, &y1);

                int byteWidth = x1 - x0;
                int byteHeight = y1 - y0;

                std::vector<unsigned char> bitmap(byteWidth * byteHeight);
                stbtt_MakeCodepointBitmapSubpixel(&font.font_info, bitmap.data(), byteWidth, byteHeight, byteWidth, font.scale, font.scale, x_shift, 0, text[i]);

                // Draw the bitmap into the image buffer
                for (int j = 0; j < byteHeight; ++j)
                {
                    for (int k = 0; k < byteWidth; ++k)
                    {
                        int px = x + x0 + k;
                        int py = startY + y0 + j;
                        if (px >= 0 && px < size.width && py >= 0 && py < size.height)
                        {
                            unsigned char alpha = bitmap[j * byteWidth + k];
                            unsigned char existingR = pixels[(py * size.width + px) * 4 + 0];
                            unsigned char existingG = pixels[(py * size.width + px) * 4 + 1];
                            unsigned char existingB = pixels[(py * size.width + px) * 4 + 2];

                            // Blend the font color with the existing pixel color
                            pixels[(py * size.width + px) * 4 + 0] = (alpha * color.r + (255 - alpha) * existingR) / 255;
                            pixels[(py * size.width + px) * 4 + 1] = (alpha * color.g + (255 - alpha) * existingG) / 255;
                            pixels[(py * size.width + px) * 4 + 2] = (alpha * color.b + (255 - alpha) * existingB) / 255;
                            pixels[(py * size.width + px) * 4 + 3] = std::max(pixels[(py * size.width + px) * 4 + 3], alpha);
                        }
                    }
                }

                x += static_cast<int>(static_cast<float>(advance) * font.scale);
                if (text[i + 1])
                    x += static_cast<int>(std::round(static_cast<int>(font.scale) * stbtt_GetCodepointKernAdvance(&font.font_info, text[i], text[i + 1])));
            }
        }

        gfx::Image resize_overlay(const gfx::Image &overlay, const geom::Size &new_size)
        {
            gfx::Image resized_overlay;
            resized_overlay.size = new_size;
            resized_overlay.channels = overlay.channels;
            resized_overlay.buffer = (uint8_t *)malloc(new_size.width * new_size.height * overlay.channels);

            stbir_pixel_layout layout = (overlay.channels == 1) ? STBIR_1CHANNEL : (overlay.channels == 2) ? STBIR_2CHANNEL
                                                                               : (overlay.channels == 3)   ? STBIR_RGB
                                                                                                           : STBIR_RGBA;

            stbir_resize_uint8_linear(overlay.buffer, overlay.size.width, overlay.size.height, 0,
                                      resized_overlay.buffer, new_size.width, new_size.height, 0, layout);

            return resized_overlay;
        }

        geom::Size calculate_new_size(const geom::Size &base_size, const geom::Size &overlay_size, float ratio)
        {
            int base_max_edge = std::max(base_size.width, base_size.height);
            int overlay_max_edge = std::max(overlay_size.width, overlay_size.height);

            float scale = (base_max_edge * ratio) / overlay_max_edge;

            return geom::Size{static_cast<int>(overlay_size.width * scale), static_cast<int>(overlay_size.height * scale)};
        }

        std::pair<int, int> calculate_overlay_position(const geom::Size &base_size, const geom::Size &overlay_size, OverlayPosition position)
        {
            int x = 0, y = 0;

            switch (position)
            {
            case OverlayPosition::Left:
                x = 0;
                y = (base_size.height - overlay_size.height) / 2;
                break;
            case OverlayPosition::Top:
                x = (base_size.width - overlay_size.width) / 2;
                y = 0;
                break;
            case OverlayPosition::Right:
                x = base_size.width - overlay_size.width;
                y = (base_size.height - overlay_size.height) / 2;
                break;
            case OverlayPosition::Bottom:
                x = (base_size.width - overlay_size.width) / 2;
                y = base_size.height - overlay_size.height;
                break;
            case OverlayPosition::TopLeft:
                x = 0;
                y = 0;
                break;
            case OverlayPosition::TopRight:
                x = base_size.width - overlay_size.width;
                y = 0;
                break;
            case OverlayPosition::BottomLeft:
                x = 0;
                y = base_size.height - overlay_size.height;
                break;
            case OverlayPosition::BottomRight:
                x = base_size.width - overlay_size.width;
                y = base_size.height - overlay_size.height;
                break;
            }

            return std::make_pair(x, y);
        }

        gfx::Image overlay_image(const gfx::Image &base, const gfx::Image &overlay, float ratio, OverlayPosition position)
        {
            geom::Size new_size = calculate_new_size(base.size, overlay.size, ratio);
            gfx::Image resized_overlay = resize_overlay(overlay, new_size);
            gfx::Image result = base; // Start with a copy of the base image

            auto [start_x, start_y] = calculate_overlay_position(base.size, resized_overlay.size, position);

            for (int y = 0; y < resized_overlay.size.height; ++y)
            {
                for (int x = 0; x < resized_overlay.size.width; ++x)
                {
                    int base_index = ((start_y + y) * base.size.width + (start_x + x)) * base.channels;
                    int overlay_index = (y * resized_overlay.size.width + x) * resized_overlay.channels;

                    // Blend the pixels (assuming RGBA)
                    uint8_t alpha = resized_overlay.buffer[overlay_index + 3];
                    for (int c = 0; c < 3; ++c)
                    {
                        result.buffer[base_index + c] = (alpha * resized_overlay.buffer[overlay_index + c] + (255 - alpha) * base.buffer[base_index + c]) / 255;
                    }
                    result.buffer[base_index + 3] = std::max(result.buffer[base_index + 3], alpha);
                }
            }

            return result;
        }

        void save_image(const gfx::Image &img, const char *file_path)
        {
            stbi_write_png(file_path, img.size.width, img.size.height, img.channels, img.buffer, img.size.width * img.channels);
        }

        bool draw_circle(gfx::Image &image, int radius, const geom::Point &center, const gfx::Color &color)
        {
            if (!image.is_valid() || radius <= 0 || image.channels < 3)
            {
                return false;
            }

            int width = image.size.width;
            int height = image.size.height;
            int channels = image.channels;

            auto blend = [](uint8_t dst, uint8_t src, float alpha) -> uint8_t {
                return static_cast<uint8_t>(dst * (1.0f - alpha) + src * alpha);
            };

            for (int y = center.y - radius; y <= center.y + radius; ++y)
            {
                for (int x = center.x - radius; x <= center.x + radius; ++x)
                {
                    int dx = x - center.x;
                    int dy = y - center.y;
                    float distance = (float)std::sqrt(dx * dx + dy * dy);
                    if (distance <= radius + 0.5f)
                    {
                        if (x >= 0 && x < width && y >= 0 && y < height)
                        {
                            int index = (y * width + x) * channels;
                            float alpha = std::min(1.0f, std::max(0.0f, 1.0f - (distance - radius)));
                            image.buffer[index] = blend(image.buffer[index], color.r, alpha);
                            image.buffer[index + 1] = blend(image.buffer[index + 1], color.g, alpha);
                            image.buffer[index + 2] = blend(image.buffer[index + 2], color.b, alpha);
                            if (channels == 4)
                            {
                                image.buffer[index + 3] = blend(image.buffer[index + 3], color.a, alpha);
                            }
                        }
                    }
                }
            }
            return true;
        }
    } // namespace img
} // namespace util
