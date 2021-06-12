#include "Disc.hpp"

#include "../math.hpp"

Disc::Disc(glm::vec3 center, glm::vec3 normal, float radius, Material material) :
    _center(center),
    _normal(normal),
    _radius(radius),
    Solid(material)
{
}

std::optional<RayHit> Disc::intersect(Ray &ray)
{
    auto t = glm::dot(_normal, (_center - ray.origin)) / glm::dot(_normal, ray.direction);
    
    if (t >= 0)
    {
        auto intersectionPoint = ray.origin + t * ray.direction;
        auto pointInCircle = glm::dot(
            intersectionPoint - _center,
            intersectionPoint - _center
        ) <= math::square(_radius);
        
        if (pointInCircle)
        {
            auto normal = calculateNormal(intersectionPoint);
            return RayHit{ glm::vec3{0.f}, normal, this, Pixel{0xFF, 0x0, 0x0} };
        }
        else
        {
            return {};
        }
    }

    return {};
}

glm::vec3 Disc::calculateNormal(glm::vec3 point) const
{
    return _normal;
}
