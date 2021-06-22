#include "Mesh.hpp"

#include <optional>

#include <glm/gtc/constants.hpp>

Mesh::Mesh() :
    _faces(),
    _center(glm::vec3()),
    Solid(Material()) {}

Mesh::Mesh(
    std::vector<Face> faces,
    glm::vec3 center,
    Material material
) :
    _faces(faces),
    _center(center),
    Solid(material)
{
}

std::optional<RayHit> Mesh::intersect(const Ray& ray)
{
    std::optional<Face> intersectedFace;
    float tIntersection = -1.f;

    for (Face face : _faces)
    {
        // Möller–Trumbore ray-triangle intersection algorithm
        // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
        glm::vec3 edge1 = face.p1 - face.p0;
        glm::vec3 edge2 = face.p2 - face.p0;
        glm::vec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);

        if (a > -glm::epsilon<float>() && a < glm::epsilon<float>())
        {
            continue;
        }

        float f = 1.f / a;
        glm::vec3 s = ray.origin - (_center + face.p0);
        float u = f * glm::dot(s, h);

        if (u < 0.f || u > 1.f)
        {
            continue;
        }

        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(ray.direction, q);
        if (v < 0.f || u + v > 1.f)
        {
            continue;
        }

        float t = f * glm::dot(edge2, q);
 
        if (t > glm::epsilon<float>() && (tIntersection <= glm::epsilon<float>() || t < tIntersection))
        {
            tIntersection = t;
            intersectedFace = face;
        }
    }

    if (auto faceIntersected = intersectedFace)
    {
        glm::vec3 intersection = ray.origin + ray.direction * tIntersection;

        glm::vec3 normal = glm::normalize(-glm::cross(
            (faceIntersected->p0 - faceIntersected->p1),
            (faceIntersected->p2 - faceIntersected->p0)
        ));

        auto isFrontFace = glm::dot(ray.direction, normal) < 0.f;
        return RayHit{ intersection, normal, shared_from_this(), tIntersection, isFrontFace };
    }

    return {};
}

glm::vec3 Mesh::calculateNormal(glm::vec3 point) const
{
    return {};
}

glm::vec3 Mesh::getCenter() const
{
    return _center;
}

std::vector<Face> Mesh::getFaces() const
{
    return _faces;
}