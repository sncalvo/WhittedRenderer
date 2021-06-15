#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"

constexpr auto HEIGHT = 2.f;
constexpr auto UP = glm::vec3(0.f, 1.f, 0.f);

class Camera
{
private:
    const float _viewportHeight;
    const float _viewportWidth;

public:
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec3 lowerLeftCorner;

    Ray createRay(float u, float v);

    Camera(float aspectRatio, float fov, glm::vec3 origin = glm::vec3(0.f), glm::vec3 lookAt = glm::vec3(0.f, 0.f, 1.f));
};
