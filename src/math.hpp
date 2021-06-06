#pragma once

#include <iostream>

namespace math
{
    constexpr auto NO_ROOT = -1;

    inline bool hasRoot(float a, float b, float c)
    {
        auto discriminant = b * b - 4 * a * c;
        return discriminant >= 0;
    }

    inline float square(float x)
    {
        return x * x;
    }

    inline float solve(float a, float b, float c)
    {
        auto discriminant = square(b) - 4 * a * c;

        if (discriminant >= 0)
        {
            return (-b - sqrt(discriminant)) / (2 * a);
        }
        else
        {
            return NO_ROOT;
        }
    }

    inline float abs(float x)
    {
        return x < 0 ? -x : x;
    }
}
