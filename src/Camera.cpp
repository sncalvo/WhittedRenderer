
#include "Camera.hpp"

Camera::Camera(float aspectRatio, float vFov, float focalLength, glm::vec3 origin) :
    _viewportHeight(2.f * glm::tan(glm::radians(vFov) / 2.f)),
    _viewportWidth(aspectRatio * _viewportHeight),
    _focalLength(focalLength),
    horizontal({ _viewportWidth, 0.f, 0.f }),
    vertical({ 0.f, _viewportHeight, 0.f }),
    origin(origin),
    lowerLeftCorner(
        _calculateLowerLeftCorner(
            origin,
            { aspectRatio * _viewportHeight, 0.f, 0.f },
            { 0.f, _viewportHeight, 0.f },
            focalLength))
{
}

Ray Camera::createRay(float u, float v)
{
    return { origin, glm::normalize(lowerLeftCorner + u * horizontal + v * vertical - origin) };
}

glm::vec3 Camera::_calculateLowerLeftCorner(glm::vec3 origin, glm::vec3 horizontal, glm::vec3 vertical, float focalLength)
{
    return origin + glm::vec3(0.f, 0.f, focalLength) - horizontal / 2.f - vertical / 2.f;
}
