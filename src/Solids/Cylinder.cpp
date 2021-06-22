﻿#include "Cylinder.hpp"

#include <algorithm>
#include <math.h>
#include "../math.hpp"

#include "Disc.hpp"

Cylinder::Cylinder() :
    _center(glm::vec3(0.f)),
    _radius(0.f),
    _height(0.f),
    Solid(Material())
{
}

Cylinder::Cylinder(glm::vec3 center, float radius, float height, Material material)
    : _center(center), _radius(radius), _height(height), Solid(material)
{
}

std::optional<RayHit> Cylinder::intersect(const Ray &ray)
{
    auto rayToViewer = ray.origin - _center;

    auto a = math::square(ray.direction.x) + math::square(ray.direction.z);
    auto b = 2 * (rayToViewer.x * ray.direction.x + rayToViewer.z * ray.direction.z);
    auto c = math::square(ray.origin.x) + math::square(ray.origin.z) +
        math::square(_center.x) + math::square(_center.z) - 2 *
        (ray.origin.x * _center.x + ray.origin.z * _center.z) - math::square(_radius);

    // TODO: Add top and bottom caps
    auto roots = math::solve(a, b, c);

    auto discTop = std::make_shared<Disc>(_center + glm::vec3(0.f, _height / 2.f, 0.f), glm::vec3(0.f, 1.f, 0.f) , _radius, _material);
    auto discBottom = std::make_shared<Disc>(_center + glm::vec3(0.f, -_height / 2.f, 0.f), glm::vec3(0.f, -1.f, 0.f), _radius, _material);

    auto topDiscIntersection = discTop->intersect(ray);
    auto bottomDiscIntersection = discBottom->intersect(ray);

    if (topDiscIntersection.has_value())
    {
        roots.push_back(topDiscIntersection->t);
    }
    if (bottomDiscIntersection.has_value())
    {
        roots.push_back(bottomDiscIntersection->t);
    }

    std::sort(roots.begin(), roots.end());

    std::optional<float> t;
    for (const auto& root : roots)
    {
        if (root < 0)
        {
            continue;
        }

        auto intersection = ray.origin + root * ray.direction;

        // TODO: Check y coordinate if we want to rotate cylinders
        if (math::abs(intersection.y - _center.y) > _height / 2)
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

    auto visibleIntersection = ray.origin + *t * ray.direction;

    auto normal = calculateNormal(visibleIntersection);
    auto isFrontFace = glm::dot(ray.direction, normal) < 0.f;

    if (topDiscIntersection.has_value() && t == topDiscIntersection->t)
    {
        return topDiscIntersection;
    }
    if (bottomDiscIntersection.has_value() && t == bottomDiscIntersection->t)
    {
        return bottomDiscIntersection;
    }

    return RayHit{ visibleIntersection, normal, shared_from_this(), *t, isFrontFace };
}

glm::vec3 Cylinder::calculateNormal(glm::vec3 point) const
{
    // TODO: Check y coordinate if we want to rotate cylinders
    auto difference = point - glm::vec3(_center.x, point.y, _center.z);
    return glm::normalize(difference);
}

glm::vec3 Cylinder::getCenter() const
{
    return _center;
}

float Cylinder::getRadius() const
{
    return _radius;
}

float Cylinder::getHeight() const
{
    return _height;
}
