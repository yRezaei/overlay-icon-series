#ifndef E316A9F6_A0BD_48DC_82DF_CD69FC032458
#define E316A9F6_A0BD_48DC_82DF_CD69FC032458

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "geom/size.h"
#include "gfx/color.h"

namespace gfx
{

    struct Image
    {
        uint8_t *buffer;
        geom::Size size;
        int channels;

        // Default constructor
        Image() : buffer(nullptr), size{0, 0}, channels{0} {}

        // Destructor
        ~Image()
        {
            if (buffer)
                free(buffer);
        }

        // Move constructor
        Image(Image &&other) noexcept
            : buffer(other.buffer), size(other.size), channels(other.channels)
        {
            other.buffer = nullptr;
            other.size = {0, 0};
            other.channels = 0;
        }

        // Move assignment operator
        Image &operator=(Image &&other) noexcept
        {
            if (this != &other)
            {
                if (buffer)
                    free(buffer);

                buffer = other.buffer;
                size = other.size;
                channels = other.channels;

                other.buffer = nullptr;
                other.size = {0, 0};
                other.channels = 0;
            }
            return *this;
        }

        // Copy constructor
        Image(const Image &other)
            : size(other.size), channels(other.channels)
        {
            if (other.buffer)
            {
                size_t dataSize = size.width * size.height * channels;
                buffer = (uint8_t *)malloc(dataSize);
                std::memcpy(buffer, other.buffer, dataSize);
            }
            else
            {
                buffer = nullptr;
            }
        }

        // Copy assignment operator
        Image &operator=(const Image &other)
        {
            if (this != &other)
            {
                if (buffer)
                    free(buffer);

                size = other.size;
                channels = other.channels;

                if (other.buffer)
                {
                    size_t dataSize = size.width * size.height * channels;
                    buffer = (uint8_t *)malloc(dataSize);
                    std::memcpy(buffer, other.buffer, dataSize);
                }
                else
                {
                    buffer = nullptr;
                }
            }
            return *this;
        }

        // Constructor to initialize with size and color
        Image(const geom::Size &size, int channels, const gfx::Color &color)
            : size(size), channels(channels)
        {
            size_t dataSize = size.width * size.height * channels;
            buffer = (uint8_t *)malloc(dataSize);
            for (int y = 0; y < size.height; ++y)
            {
                for (int x = 0; x < size.width; ++x)
                {
                    int index = (y * size.width + x) * channels;
                    buffer[index] = color.r;
                    buffer[index + 1] = color.g;
                    buffer[index + 2] = color.b;
                    if (channels == 4)
                    {
                        buffer[index + 3] = color.a;
                    }
                }
            }
        }

        // Function to check if the image is valid
        bool is_valid() const
        {
            return buffer && size.width > 0 && size.height > 0 && channels > 0;
        }
    };

} // namespace gfx

#endif /* E316A9F6_A0BD_48DC_82DF_CD69FC032458 */
