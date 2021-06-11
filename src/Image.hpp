#pragma once

#include <memory>

#include "Pixel.hpp"

class Image
{
private:
    static const auto CHANNELS = 3;
    static const auto HEIGHT = 1080;
    static const auto WIDTH = 1920;

    unsigned int _height, _width;
    std::unique_ptr<Pixel[]> _buffer;

public:
    Image(unsigned int height = HEIGHT, unsigned int width = WIDTH);

    unsigned int getWidth();

    unsigned int getHeight();

    void write(const char* fileName);

    Pixel& operator[](std::size_t index) const;

    float aspectRatio() const;
};
