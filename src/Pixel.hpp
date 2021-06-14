#pragma once

struct Pixel
{
    unsigned char r, g, b;
    Pixel operator+(const Pixel& other)
    {
        Pixel newPixel = Pixel{
            (unsigned char)(std::min(r + other.r, 255)),
            (unsigned char)(std::min(g + other.g, 255)),
            (unsigned char)(std::min(b + other.b, 255)),
        };
        return newPixel;
    }

    Pixel& operator+=(const Pixel& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;

        return *this;
    }

    Pixel operator*(const int factor)
    {
        return Pixel{
            static_cast<unsigned char>(r * factor),
            static_cast<unsigned char>(g * factor),
            static_cast<unsigned char>(b * factor),
        };
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
