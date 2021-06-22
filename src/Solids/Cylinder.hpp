#pragma once

#define GLM_SWIZZLE

#include <glm/glm.hpp>

#include "Solid.hpp"

class Cylinder : public Solid
{
private:
    float _radius, _height;
    glm::vec3 _center;

    std::vector<float> _intersectWithCaps(const Ray& ray) const;

public:
    Cylinder();
    Cylinder(glm::vec3 center, float radius, float height, Material material);
    std::optional<RayHit> intersect(const Ray &ray) override;
    glm::vec3 calculateNormal(glm::vec3 point) const override;
    glm::vec3 getCenter() const;
    float getRadius() const;
    float getHeight() const;
};
