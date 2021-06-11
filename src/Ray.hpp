#pragma once

#include <glm/glm.hpp>

#include "Pixel.hpp"
#include "Solids/Solid.hpp"

struct Ray
{
    glm::vec3 origin, direction;

    Pixel calculateColor(Material material, glm::vec3 normal, float distance, RayHit hit);
};

struct RayHit
{
    glm::vec3 position;
    Pixel pixel;
};
