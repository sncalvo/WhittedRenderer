#include <vector>

#include <glm/glm.hpp>

#include "Ray.hpp"

#include "Light.hpp"

float attenuation(float distance)
{
    return 1.f;
}


Pixel Ray::calculateColor(Material material, glm::vec3 normal, float distance, RayHit hit)
{
    Light light{ glm::vec3(1.f), .5f, 1.f };

    std::vector<Light> lights;
    lights.push_back(light);

    // Ambient light
    glm::vec3 ambientColor = 1.f * material.diffuseColor;

    for (Light light : lights)
    {
        // Diffuse light
        auto diffuse = material.diffuseColor * glm::dot(normal, hit.position);

        // Specular light
        auto refect = glm::reflect(direction, normal);

        // TODO: See specular factor (pow)
        auto specularFactor =  1.f;
        auto specular = material.specularColor * pow(glm::dot(refect, direction), specularFactor);

        // TODO: Check S_i
        auto s = 1.f;

        auto lightColor = s * (diffuse + specular) * attenuation(distance) * light.intensity;
    }
}
