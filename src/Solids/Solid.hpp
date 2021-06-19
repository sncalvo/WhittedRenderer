#pragma once

#include <memory>
#include <optional>

#include "../Ray.hpp"
#include "../Pixel.hpp"

struct Material
{
    glm::vec3 diffuseColor, specularColor;
    float diffuse, specular, transparency, refractionIndex;
    Material() :
        diffuseColor(glm::vec3(0.f)),
        specularColor(glm::vec3(0.f)),
        diffuse(0.f),
        specular(0.f),
        transparency(0.f),
        refractionIndex(0.f) {};
    Material(
        glm::vec3 diffuseColor,
        glm::vec3 specularColor,
        float diffuse,
        float specular,
        float transparency,
        float refractionIndex
    ) :
        diffuseColor(diffuseColor),
        specularColor(specularColor),
        diffuse(diffuse),
        specular(specular),
        transparency(transparency),
        refractionIndex(refractionIndex) {};
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
