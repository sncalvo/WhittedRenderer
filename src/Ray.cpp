#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/constants.hpp>

#include "Solids/Solid.hpp"
#include "Ray.hpp"
#include "Light.hpp"

auto constexpr MAX_DEPTH = 5;
auto constexpr BACKGROUND_COLOR = Pixel{ 0, 0, 0 };

float attenuation(float distance)
{
    // TODO: Perform calculation
    return 1.f;
}

Pixel Ray::calculateColor(std::vector<std::unique_ptr<Solid>> &solids, int depth)
{
    // TODO: Move to another function
    std::optional<RayHit> hit;
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
        return _calculateColor(*(hit), solids, depth);
    }
    else
    {
        return BACKGROUND_COLOR;
    }
}

Pixel Ray::_calculateColor(RayHit hit, std::vector<std::unique_ptr<Solid>> &solids, int depth)
{
    // TODO: Create singleton with list of lights
    Light light{ glm::vec3(-.5f, 1.f, 1.f), glm::vec3(1.f), 1.f, .5f };

    std::vector<Light> lights;
    lights.push_back(light);

    const Material& material = hit.solid->getMaterial();

    // Ambient light
    glm::vec3 ambientColor = .1f * material.diffuseColor;
    glm::vec3 lightColor;

    for (const Light &light : lights)
    {
        auto directionToLight = glm::normalize(light.position - hit.position);

        // Diffuse color
        auto diffuse = material.diffuseColor * std::max(glm::dot(hit.normal, directionToLight), 0.f);

        // Specular color
        auto reflection = glm::normalize(glm::reflect(directionToLight, hit.normal));
        auto shininess = 64.f; // TODO: See specular factor (pow)
        auto specular = material.specularColor * pow(std::max(glm::dot(-reflection, glm::normalize(origin - hit.position)), 0.f), shininess);

        // TODO: Check S_i
        auto s = 1.f;

        auto distanceToLight = glm::l2Norm(light.position, hit.position);

        // TODO: If more than one light we need to accumulate
        lightColor = s * attenuation(distanceToLight) * light.intensity * (diffuse + specular);
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
        auto reflection = glm::reflect(glm::normalize(direction), hit.normal);
        Ray ray{ hit.position - glm::vec3(glm::epsilon<float>()) * direction, reflection };
        color = color + ray.calculateColor(solids, depth + 1);
    }

    if (material.transmission > 0)
    {
        // TODO: If angle means total reflection don't do anything
        auto theta = asin(material.transmission / 1.f); // TODO: Calculate transmission of current medium
        auto rotationMatrix = glm::rotate(glm::mat4(1.f), theta, direction);
        auto transmissionDirection = rotationMatrix * glm::vec4(direction, 1.f);
        Ray ray{ hit.position, glm::vec3(transmissionDirection) };
        color = color + ray.calculateColor(solids, depth + 1);
    }

    return color;
}
