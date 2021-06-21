#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Pixel.hpp"

class Solid;
class Material;
class Scene;

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
    std::tuple<glm::vec3, Material> calculateColorAndMaterial(std::vector<std::shared_ptr<Solid>> &solids, int depth, Scene scene);
private:
    glm::vec3 _calculateColor(RayHit hit, std::vector<std::shared_ptr<Solid>> &solids, int depth, Scene scene);
    std::vector<RayHit> _calculatePathIntersections(std::vector<std::shared_ptr<Solid>> &solids) const;
};
