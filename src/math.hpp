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

    inline std::vector<float> solve(float a, float b, float c)
    {
        auto discriminant = square(b) - 4 * a * c;

        std::vector<float> roots;

        if (discriminant >= 0)
        {
            if (discriminant == 0)
            {
                roots.push_back(-b / (2 * a));
            }
            else
            {
                roots.push_back((-b - std::sqrt(discriminant)) / (2 * a));
                roots.push_back((-b + std::sqrt(discriminant)) / (2 * a));
            }
        }
        return roots;
    }

    inline float abs(float x)
    {
        return x < 0 ? -x : x;
    }
}
