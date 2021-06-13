#include <cmath>
#include <iterator>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/constants.hpp>

#include "Solids/Solid.hpp"
#include "Ray.hpp"
#include "Light.hpp"

auto constexpr MAX_DEPTH = 2;
auto constexpr BACKGROUND_COLOR = Pixel{ 0, 0, 0 };

float attenuation(float distance)
{
    return 1.f / glm::pow(distance, 2.f);
}

Pixel Ray::calculateColor(std::vector<std::shared_ptr<Solid>> &solids, int depth)
{
    // TODO: Move to another function
    std::optional<RayHit> hit;
    // TODO: Use t of parametric line to be able to move camera
    unsigned int nearestIntersectionZ = UINT32_MAX;
    for (auto& solid : solids)
    {
        if (auto intersection = solid->intersect(*this); intersection.has_value()) // TODO: See if we can get rid of the has_value()
        {
            if (nearestIntersectionZ > intersection->position.z)
            {
                nearestIntersectionZ = intersection->position.z;
                hit = intersection;
            }
        }
    }

    if (hit.has_value())
    {
        return _calculateColor(*hit, solids, depth);
    }
    else
    {
        return BACKGROUND_COLOR;
    }
}

Pixel Ray::_calculateColor(RayHit hit, std::vector<std::shared_ptr<Solid>> &solids, int depth)
{
    // TODO: Create singleton with list of lights
    Light light{ glm::vec3(1.f, 0.f, 4.f), glm::vec3(1.f), 1.f, .5f };
    Light light2{ glm::vec3(-1.f, 0.f, 6.f), glm::vec3(1.f), 1.f, .5f };

    std::vector<Light> lights;
    lights.push_back(light);
    lights.push_back(light2);

    const Material& material = hit.solid->getMaterial();

    // Ambient light
    glm::vec3 ambientColor = .1f * material.diffuseColor;
    glm::vec3 lightColor = glm::vec3(0.f);

    for (const auto &light : lights)
    {
        auto directionToLight = glm::normalize(light.position - hit.position);

        // Diffuse color
        auto diffuse = material.diffuse * material.diffuseColor * std::max(glm::dot(hit.normal, directionToLight), 0.f);

        // Specular color
        auto reflection = glm::normalize(glm::reflect(directionToLight, hit.normal));
        auto shininess = 64.f; // TODO: See specular factor (pow)
        auto specular = material.specular * material.specularColor * pow(std::max(glm::dot(-reflection, glm::normalize(origin - hit.position)), 0.f), shininess);

        float s = 1.f;
        Ray ray{ hit.position - 100.f * glm::vec3(glm::epsilon<float>()) * direction, directionToLight };
        auto intersectionSolids = ray._calculateLightPathIntersections(solids);
        for (const auto& intersectionSolid : intersectionSolids)
        {
            s *= intersectionSolid->getMaterial().transparency;
            if (s == 0.f)
            {
                break;
            }
        }

        auto distanceToLight = glm::l2Norm(light.position, hit.position);
        auto lightAttenuation = attenuation(distanceToLight);

        lightColor += s * lightAttenuation * light.intensity * (diffuse + specular);
    }

    auto ambientAndLight = glm::clamp(
        ambientColor + lightColor,
        glm::vec3(0.f),
        glm::vec3(1.f)
    );
    auto color = Pixel{
        (unsigned char) (ambientAndLight.x * 255.f),
        (unsigned char) (ambientAndLight.y * 255.f),
        (unsigned char) (ambientAndLight.z * 255.f)
    };

    if (!(depth < MAX_DEPTH))
    {
        return color;
    }

    if (material.specular > 0)
    {
        auto reflection = glm::reflect(direction, hit.normal);
        Ray ray{
            hit.position - 100.f * glm::vec3(glm::epsilon<float>()) * direction,
            glm::normalize(reflection)
        };
        color = color + ray.calculateColor(solids, depth + 1) * material.specular;
    }

    if (material.transparency > 0)
    {
        auto isCriticalAngle = false;
        auto cosTheta = std::min(glm::dot(-direction, hit.normal), 1.f);
        auto sinTheta = std::sqrt(1.f - cosTheta * cosTheta);
        auto refractionRatio = (1.f / material.refractionIndex);
        if (!(refractionRatio * sinTheta > 1.f))
        {
            auto refractionPerpendicular = (1.f / 2.f) *
                (direction + cosTheta * hit.normal);
            auto refractionParallel = -glm::sqrt(
                std::abs(1.f - glm::dot(refractionPerpendicular, refractionPerpendicular))
            ) * hit.normal;
            Ray ray{
                hit.position + 100.f * glm::vec3(glm::epsilon<float>()) * direction,
                glm::normalize(refractionPerpendicular + refractionParallel)
            };
            color = color + ray.calculateColor(solids, depth + 1) * material.transparency;
        }
    }

    return color;
}

std::vector<std::shared_ptr<Solid>> Ray::_calculateLightPathIntersections(std::vector<std::shared_ptr<Solid>> &solids) const
{
    std::vector<RayHit> intersections;
    for (const auto& solid : solids)
    {
        if (auto intersection = solid->intersect(*this); intersection.has_value())
        {
            auto hit = *intersection;
            intersections.push_back(hit);
        }
    }
    std::vector<std::shared_ptr<Solid>> intersectionSolids;
    std::sort(intersections.begin(), intersections.end());
    std::transform(
        intersections.begin(),
        intersections.end(),
        std::back_inserter(intersectionSolids),
        [](RayHit hit) -> std::shared_ptr<Solid>
        {
            return hit.solid;
        }
    );
    return intersectionSolids;
}