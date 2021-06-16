#include "Sphere.hpp"

#include "../math.hpp"

Sphere::Sphere() :
    _center(glm::vec3(0.f)),
    _radius(1.f),
    Solid(
        Material{
            glm::vec3(1.f),
            glm::vec3(1.f),
            1.f,
            0.f,
            0.f,
            0.f
        }
    )
{
}

Sphere::Sphere(glm::vec3 center, float radius, Material material) : _center(center), _radius(radius), Solid(material)
{
}

std::optional<RayHit> Sphere::intersect(const Ray &ray)
{
    auto rayToViewer = ray.origin - _center;

    auto a = glm::dot(ray.direction, ray.direction);
    auto b = 2 * glm::dot(ray.direction, rayToViewer);
    auto c = glm::dot(rayToViewer, rayToViewer) - math::square(_radius);

    auto roots = math::solve(a, b, c);

    std::optional<float> t;
    for (const auto& root : roots)
    {
        if (root < 0)
        {
            continue;
        }

        t = root;
        break;
    }

    if (!t.has_value())
    {
        return {};
    }

    auto intersection = ray.origin + (*t) * ray.direction;
    auto normal = calculateNormal(intersection);
    auto isFrontFace = glm::dot(ray.direction, normal) < 0.f;
    return RayHit{ intersection, normal, shared_from_this(), (*t), isFrontFace };
}

glm::vec3 Sphere::calculateNormal(glm::vec3 point) const
{
    return glm::normalize(point - _center);
}

glm::vec3 Sphere::getCenter() const
{
    return _center;
}

float Sphere::getRadius() const
{
    return _radius;
}
