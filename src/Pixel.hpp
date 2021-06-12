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

    Pixel& operator+=(const Pixel& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;

        return *this;
    }

    Pixel operator/(const int factor)
    {
        return Pixel{
            static_cast<unsigned char>(r / factor),
            static_cast<unsigned char>(g / factor),
            static_cast<unsigned char>(b / factor),
        };
    }
};
