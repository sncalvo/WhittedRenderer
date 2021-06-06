#include "Sphere.hpp"

#include "../math.hpp"

Sphere::Sphere(glm::vec3 center, float radius) : _center(center), _radius(radius)
{
}

bool Sphere::intersect(Ray ray)
{
    auto rayToViewer = ray.origin - _center;

    auto a = glm::dot(ray.direction, ray.direction);
    auto b = 2 * glm::dot(ray.direction, rayToViewer);
    auto c = glm::dot(rayToViewer, rayToViewer) - _radius * _radius;
    return math::hasRoot(a, b, c);
}
