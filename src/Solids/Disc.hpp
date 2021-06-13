#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Solid.hpp"

class Disc : public Solid
{
private:
    glm::vec3 _center, _normal;
    float _radius;

public:
    Disc(glm::vec3 center, glm::vec3 normal, float radius, Material material);

    std::optional<RayHit> intersect(const Ray& ray) override;
    glm::vec3 calculateNormal(glm::vec3 point) const override;
};
