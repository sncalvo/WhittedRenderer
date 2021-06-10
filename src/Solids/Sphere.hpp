#pragma once

#include <glm/glm.hpp>

#include "Solid.hpp"

class Sphere : public Solid
{
private:
    glm::vec3 _center;
    float _radius;

public:
    Sphere(glm::vec3 center, float radius);

    bool intersect(Ray ray) override;
};
