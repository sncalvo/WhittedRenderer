#include "Cylinder.hpp"

#include <math.h>
#include "../math.hpp"
#include "../Log.hpp"

Cylinder::Cylinder(glm::vec3 center, float radius, float height)
    : _center(center), _radius(radius), _height(height)
{
}

bool Cylinder::intersect(Ray ray)
{
    auto rayToViewer = ray.origin - _center;

    auto a = math::square(ray.direction.x) + math::square(ray.direction.z);
    auto b = 2 * (rayToViewer.x * ray.direction.x + rayToViewer.z * ray.direction.z);
    auto c = math::square(ray.origin.x) + math::square(ray.origin.z) +
        math::square(_center.x) + math::square(_center.z) - 2 *
        (ray.origin.x * _center.x + ray.origin.z * _center.z) - math::square(_radius);

    auto t = math::solve(a, b, c);
    if (t < 0)
    {
        // TODO: Check for top intersection
        return false;
    }

    auto y = ray.origin.y + t * ray.direction.y;

    if (math::abs(y - _center.y) >= _height / 2)
    {
        return false;
    }

    return true;
}
