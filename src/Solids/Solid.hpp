#pragma once

#include <memory>
#include <optional>

#include "../Ray.hpp"
#include "../Pixel.hpp"

struct Material
{
    glm::vec3 diffuseColor, specularColor;
    float diffuse, specular, transparency, refractionIndex;
};

class Solid : public std::enable_shared_from_this<Solid>
{
protected:
    Material _material;
    Solid(Material material) : _material(material)
    {
    }

public:
    virtual ~Solid() {};
    virtual std::optional<RayHit> intersect(const Ray& ray) = 0;
    virtual glm::vec3 calculateNormal(glm::vec3 point) const = 0;
    Material getMaterial() const
    {
        return _material;
    }
};
