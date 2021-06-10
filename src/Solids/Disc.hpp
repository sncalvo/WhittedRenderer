#pragma once

#include <glm/glm.hpp>

#include "Solid.hpp"

class Disc : public Solid
{
private:
    glm::vec3 _center, _normal;
    float _radius;

public:
    Disc(glm::vec3 center, glm::vec3 normal, float radius);

    bool intersect(Ray ray) override;
};
