#pragma once

#include <iostream>

namespace math
{
    inline bool hasRoot(float a, float b, float c)
    {
        float discriminant = b * b - 4 * a * c;
        return discriminant >= 0;
    }
}
