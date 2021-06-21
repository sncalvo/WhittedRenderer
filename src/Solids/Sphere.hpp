#pragma once

#include <glm/glm.hpp>

#include "Solid.hpp"

class Sphere : public Solid
{
private:
    glm::vec3 _center;
    float _radius;

public:
    Sphere();
    Sphere(glm::vec3 center, float radius, Material material);

    std::optional<RayHit> intersect(const Ray &ray) override;
    glm::vec3 calculateNormal(glm::vec3 point) const override;
    glm::vec3 getCenter() const;
    float getRadius() const;
};
