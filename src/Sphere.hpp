#pragma once

#include <glm/glm.hpp>
#include "math.hpp"

struct Ray
{
    glm::vec3 origin, direction;
};

class Solid
{
public:
    virtual bool intersect(Ray ray) = 0;
};

class Sphere : public Solid
{
private:
    glm::vec3 _center;
    float _radius;

public:
    Sphere(glm::vec3 center, float radius) : _center(center), _radius(radius) {}

    bool intersect(Ray ray)
    {
        glm::vec3 rayToViewer = ray.origin - _center;
        
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2 * glm::dot(ray.direction, rayToViewer);
        float c = glm::dot(rayToViewer, rayToViewer)  - _radius * _radius;
        return math::hasRoot(a, b, c);
    }
};
