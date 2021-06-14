#include "Image.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <StbImage/stb_image_write.h>

Image::Image(unsigned int width, unsigned int height) :
    _width(width),
    _height(height),
    _buffer(std::make_unique<Pixel[]>((size_t) height * width))
{
}

unsigned int Image::getWidth()
{
    return _width;
}

unsigned int Image::getHeight()
{
    return _height;
}

void Image::write(const char* fileName)
{
    stbi_write_png(fileName, _width, _height, CHANNELS, _buffer.get(), CHANNELS * _width);
}

Pixel& Image::operator[](std::size_t index) const { return _buffer[index]; }

float Image::aspectRatio() const
{
    return (float)_width / (float)_height;
}
