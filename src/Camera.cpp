#include <iostream>
#include "Camera.hpp"


Camera::Camera(float aspectRatio, float vFov, glm::vec3 origin, glm::vec3 lookAt) :
    _viewportHeight(2.f * glm::tan(glm::radians(vFov) / 2.f)),
    _viewportWidth(aspectRatio * _viewportHeight),
    direction(glm::normalize(lookAt - origin)),
    origin(origin)
{
    auto xDirection = glm::normalize(glm::cross(UP, direction));
    auto yDirection = glm::cross(direction, xDirection);
    horizontal = _viewportWidth * xDirection;
    vertical = _viewportHeight * yDirection;
    lowerLeftCorner = origin - horizontal / 2.f - vertical / 2.f + direction;
}

Camera::Camera() : Camera(16.f / 9.f, 45.f, glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f))
{
}

Ray Camera::createRay(float u, float v)
{
    return { origin, glm::normalize(lowerLeftCorner + u * horizontal + v * vertical - origin) };
}
