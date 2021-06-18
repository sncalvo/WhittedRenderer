#include "Image.hpp"

#include <glm/gtx/norm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <StbImage/stb_image_write.h>

Image::Image(unsigned int width, unsigned int height) :
    _width(width),
    _height(height),
    _colorBuffer(std::make_unique<glm::vec3[]>((size_t) height * width)),
    _pixelBuffer(std::make_unique<Pixel[]>((size_t) height * width))
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

void Image::write(const char* fileName, glm::vec3 maxColor, unsigned int samplesPerPixel)
{
    _performGammaCorrection(maxColor, samplesPerPixel);
    stbi_write_png(fileName, _width, _height, CHANNELS, _pixelBuffer.get(), CHANNELS * _width);
}

glm::vec3& Image::operator[](std::size_t index) const { return _colorBuffer[index]; }

float Image::aspectRatio() const
{
    return (float)_width / (float)_height;
}

void Image::_performGammaCorrection(glm::vec3 maxValue, unsigned int samplesPerPixel)
{
    auto maxNorm = glm::l2Norm(maxValue);

    for (unsigned int colorIndex = 0; colorIndex < getWidth() * getHeight(); colorIndex++)
    {
        glm::vec3 currentColor = _colorBuffer[colorIndex];
        currentColor /= samplesPerPixel;
        currentColor /= maxNorm;
        currentColor = glm::sqrt(currentColor);
        _pixelBuffer[colorIndex] = Pixel{
            (unsigned char)(currentColor.x * 255.f),
            (unsigned char)(currentColor.y * 255.f),
            (unsigned char)(currentColor.z * 255.f)
        };
    }
}
