#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Pixel.hpp"

class Solid;

struct RayHit
{
    glm::vec3 position, normal;
    std::shared_ptr<Solid> solid;
    float t = 0.f; // t of ray's parametric line
    bool isFrontFace;

    bool operator<(const RayHit& other)
    {
        return t < other.t;
    }
};

struct Ray
{
    glm::vec3 origin, direction;
    Pixel calculateColor(std::vector<std::shared_ptr<Solid>> &solids, int depth);
private:
    Pixel _calculateColor(RayHit hit, std::vector<std::shared_ptr<Solid>> &solids, int depth);
    std::vector<std::shared_ptr<Solid>> _calculateLightPathIntersections(std::vector<std::shared_ptr<Solid>> &solids) const;
};
