#include <cmath>
#include <iterator>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/constants.hpp>

#include "Solids/Solid.hpp"
#include "Ray.hpp"
#include "Light.hpp"
#include "math.hpp"

#include "Log.hpp"

auto constexpr MAX_DEPTH = 2;
auto constexpr BACKGROUND_COLOR = glm::vec3(0.f);

float attenuation(float distance)
{
    return 1.f / glm::pow(distance, 2.f);
}

std::tuple<glm::vec3, Material> Ray::calculateColorAndMaterial(std::vector<std::shared_ptr<Solid>> &solids, int depth)
{
    /* Returns color in not normalized format */

    std::vector<RayHit> hits = _calculatePathIntersections(solids);

    if (hits.size() > 0)
    {
        return std::tuple(_calculateColor(hits[0], solids, depth), hits[0].solid->getMaterial());
    }
    else
    {
        return std::tuple(BACKGROUND_COLOR, Material());
    }
}

glm::vec3 Ray::_calculateColor(RayHit hit, std::vector<std::shared_ptr<Solid>> &solids, int depth)
{
    // TODO: Create singleton with list of lights
    Light light{ glm::vec3(0.f, 0.f, 4.f), glm::vec3(1.f), 1.f, .1f };
    //Light light2{ glm::vec3(0.f, 1.f, 5.f), glm::vec3(1.f), 1.f, .1f };

    std::vector<Light> lights;
    lights.push_back(light);
    //lights.push_back(light2);

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
        auto reflection = -glm::normalize(glm::reflect(directionToLight, hit.normal));
        auto shininess = 64.f; // TODO: See specular factor (pow)
        auto specular = 0.f * material.specular * material.specularColor * glm::pow(
            std::max(
                glm::dot(reflection, glm::normalize(origin - hit.position)),
                0.f
            ),
            shininess
        );

        auto s = 1.f;
        Ray ray{ hit.position - 1000.f * glm::vec3(glm::epsilon<float>()) * direction, directionToLight };
        auto intersectionInLightsPath = ray._calculatePathIntersections(solids);
        for (const auto& intersection : intersectionInLightsPath)
        {
            s *= intersection.solid->getMaterial().transparency;
            if (s == 0.f)
            {
                break;
            }
        }

        auto distanceToLight = glm::l2Norm(light.position, hit.position);
        auto lightAttenuation = attenuation(distanceToLight);
        lightColor += s * lightAttenuation / light.decay * light.intensity * (diffuse + specular);
    }

    auto color = ambientColor + lightColor;

    if (depth >= MAX_DEPTH)
    {
        return color;
    }

    if (material.specular > 0)
    {
        auto reflection = glm::normalize(glm::reflect(direction, hit.normal));
        Ray ray{
            hit.position - 1000.f * glm::vec3(glm::epsilon<float>()) * direction,
            reflection
        };
        color += std::get<0>(ray.calculateColorAndMaterial(solids, depth + 1)) * material.specular;
    }

    if (material.transparency > 0)
    {
        auto normalPointingToRay = hit.isFrontFace ? hit.normal : -hit.normal;
        auto cosTheta = std::min(glm::dot(-direction, normalPointingToRay), 1.f);
        auto sinTheta = std::sqrt(1.f - math::square(cosTheta));
        auto refractionRatio = hit.isFrontFace ? (1.f / material.refractionIndex) : material.refractionIndex;
        if (refractionRatio * sinTheta <= 1.f)
        {
            auto refractionPerpendicular = refractionRatio *
                (direction + cosTheta * normalPointingToRay);
            auto refractionParallel = -glm::sqrt(
                std::abs(1.f - glm::dot(refractionPerpendicular, refractionPerpendicular))
            ) * normalPointingToRay;
            auto refractionDirection = refractionPerpendicular + refractionParallel;
            Ray ray{
                hit.position + 1000.f * glm::vec3(glm::epsilon<float>()) * direction,
                refractionDirection
            };
            color += std::get<0>(ray.calculateColorAndMaterial(solids, depth + 1)) * material.transparency;
        }
    }

    return color;
}

std::vector<RayHit> Ray::_calculatePathIntersections(std::vector<std::shared_ptr<Solid>> &solids) const
{
    /* Returns vector of hits sorted in ascending distance to ray.origin */

    std::vector<RayHit> intersections;
    for (const auto& solid : solids)
    {
        if (auto intersection = solid->intersect(*this))
        {
            auto& hit = *intersection;
            intersections.push_back(hit);
        }
    }
    std::sort(intersections.begin(), intersections.end());
    return intersections;
}
