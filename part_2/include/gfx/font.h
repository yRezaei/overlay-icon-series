#ifndef B372048D_C1C0_4072_9C6A_D345D8E0E765
#define B372048D_C1C0_4072_9C6A_D345D8E0E765

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

#include "stb/stb_truetype.h"

namespace gfx
{

    struct Font
    {
        stbtt_fontinfo font_info;
        std::vector<unsigned char> font_buffer;
        float scale;

        // Public constructor to allow initialization
        Font() : scale(0.0f) {}

        // Deleted copy constructor and copy assignment operator
        Font(const Font &) = delete;
        Font &operator=(const Font &) = delete;

        // Public move constructor and move assignment operator
        Font(Font &&other) noexcept = default;
        Font &operator=(Font &&other) noexcept = default;

        // Static function to create and initialize a Font from a file
        static std::shared_ptr<Font> load_from_file(const std::string &font_path, float font_height)
        {
            auto font = std::make_shared<Font>();

            // Open the file
            std::ifstream ifs(font_path, std::ios::binary | std::ios::ate);
            if (!ifs)
            {
                std::cerr << "Failed to open font file." << std::endl;
                return nullptr;
            }

            // Get the size of the file
            std::streamsize size = ifs.tellg();
            ifs.seekg(0, std::ios::beg);

            // Resize buffer to fit the font file
            font->font_buffer.resize(size);

            // Read the file into the buffer
            if (!ifs.read((char *)font->font_buffer.data(), size))
            {
                std::cerr << "Failed to read font file." << std::endl;
                return nullptr;
            }

            // Initialize the font
            if (!stbtt_InitFont(&font->font_info, font->font_buffer.data(), stbtt_GetFontOffsetForIndex(font->font_buffer.data(), 0)))
            {
                std::cerr << "Failed to initialize font." << std::endl;
                return nullptr;
            }

            // Calculate the font scale
            font->scale = stbtt_ScaleForPixelHeight(&font->font_info, font_height);

            return font;
        }
    };

} // namespace gfx

#endif /* B372048D_C1C0_4072_9C6A_D345D8E0E765 */
