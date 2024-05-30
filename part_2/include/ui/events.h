#ifndef FED922C7_C864_4868_8726_D8B796125C2B
#define FED922C7_C864_4868_8726_D8B796125C2B

#include <cstdint>
#include "geom/point.h"

namespace ui
{
    // Mouse-related structures and enums
    enum class MouseEvent : uint8_t
    {
        NONE,
        MOVED,
        PRESSED,
        RELEASED,
        DB_CLICK
    };

    enum class MouseButton : uint8_t
    {
        NONE,
        LEFT,
        RIGHT,
        MIDDLE
    };

    struct MouseInfo
    {
        MouseEvent ev;
        geom::Point pos;
        MouseButton btn;
    };
}

#endif /* FED922C7_C864_4868_8726_D8B796125C2B */
