#pragma once

#include "Solid.hpp"
#include "../Ray.hpp"
#include "../Transform.hpp"

class Cone : public Solid
{
public:
    bool intersect(Ray ray) override;
    Transform transform;
};
