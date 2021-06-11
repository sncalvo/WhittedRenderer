#include "Sphere.hpp"

#include "../math.hpp"

Sphere::Sphere(glm::vec3 center, float radius, Material material) : _center(center), _radius(radius), Solid(material)
{
}

std::optional<RayHit> Sphere::intersect(Ray &ray)
{
    auto rayToViewer = ray.origin - _center;

    auto a = glm::dot(ray.direction, ray.direction);
    auto b = 2 * glm::dot(ray.direction, rayToViewer);
    auto c = glm::dot(rayToViewer, rayToViewer) - _radius * _radius;

    auto t = math::solve(a, b, c);

    if (t != math::NO_ROOT)
    {
        auto intersection = ray.origin + t * ray.direction;
        return RayHit{ intersection, Pixel{0xFF, 0x0, 0x0} };
    }
    else
    {
        return {};
    }
}
