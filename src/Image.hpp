#pragma once

#include <memory>

struct Pixel
{
    unsigned char r, g, b;
};

class Image
{
private:
    static const int CHANNELS = 3;
    static const int HEIGHT = 1080;
    static const int WIDTH = 1920;

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
