#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Pixel.hpp"

class Image
{
private:
    static const auto CHANNELS = 3;
    static const auto HEIGHT = 1080;
    static const auto WIDTH = 1920;

    unsigned int _height, _width;
    std::unique_ptr<glm::vec3[]> _colorBuffer;
    std::unique_ptr<Pixel[]> _pixelBuffer;

    void _performGammaCorrection(glm::vec3 maxValue, unsigned int samplesPerPixel);

public:
    Image(unsigned int width = WIDTH, unsigned int height = HEIGHT);

    unsigned int getWidth();

    unsigned int getHeight();

    void write(const char* fileName, glm::vec3 maxColor, unsigned int samplesPerPixel);

    glm::vec3& operator[](std::size_t index) const;

    float aspectRatio() const;
};
