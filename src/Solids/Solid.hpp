#pragma once

#include "../Ray.hpp"

class Solid
{
public:
    virtual bool intersect(Ray ray) = 0;
};
