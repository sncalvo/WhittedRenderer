#pragma once

#include <optional>

#include "../Ray.hpp"
#include "../Pixel.hpp"

struct Material
{
    glm::vec3 diffuseColor, specularColor, transmissionColor;
    float diffuse, specular, transmission;
};

class Solid
{
protected:
    Material _material;
    Solid(Material material) : _material(material)
    {
    }

public:
    virtual ~Solid() {};
    virtual std::optional<RayHit> intersect(Ray& ray) = 0;
    virtual glm::vec3 calculateNormal(glm::vec3 point) const = 0;
    Material getMaterial() const
    {
        return _material;
    }
};
