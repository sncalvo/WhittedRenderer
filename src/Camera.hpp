#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"

constexpr auto HEIGHT = 2.f;

class Camera
{
private:
    const float _viewportHeight;
    const float _viewportWidth;
    const float _focalLength = 1.f;
    
    glm::vec3 _calculateLowerLeftCorner(glm::vec3 origin, glm::vec3 horizontal, glm::vec3 vertical, float focalLength);

public:
    const glm::vec3 horizontal;
    const glm::vec3 vertical;
    const glm::vec3 origin{ 0.f };
    const glm::vec3 lowerLeftCorner;

    inline glm::vec3 pixelRayDirection(unsigned int column, unsigned int row, unsigned int width, unsigned int height)
    {
        auto u = float(column) / float(width - 1);
        auto v = float(row) / float(height - 1);
        return lowerLeftCorner + u * horizontal + v * vertical - origin;
    }

    Ray createRay(float u, float v);

    Camera(float aspectRatio, float fov, float focalLength, glm::vec3 origin);
};
