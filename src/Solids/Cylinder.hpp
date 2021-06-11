#pragma once

#define GLM_SWIZZLE

#include <glm/glm.hpp>

#include "Solid.hpp"

class Cylinder : public Solid
{
private:
    float _radius, _height;
    glm::vec3 _center;

public:
    Cylinder(glm::vec3 center, float radius, float height);
    std::optional<RayHit> intersect(Ray &ray) override;
};
