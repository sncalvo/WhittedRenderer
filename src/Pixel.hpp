#pragma once

struct Pixel
{
    unsigned char r, g, b;
    Pixel operator+(const Pixel& other)
    {
        return Pixel{
            (unsigned char) (r + other.r),
            (unsigned char) (g + other.g),
            (unsigned char) (b + other.b),
        };
    }
};
