#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Pixel.hpp"

class Solid;

struct RayHit
{
    glm::vec3 position, normal;
    Solid *solid;
    Pixel pixel;
};

struct Ray
{
    glm::vec3 origin, direction;
    Pixel calculateColor(std::vector<std::unique_ptr<Solid>> &solids, int depth);
private:
    Pixel _calculateColor(RayHit hit, std::vector<std::unique_ptr<Solid>> &solids, int depth);
};
