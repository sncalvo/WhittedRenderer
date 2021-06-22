#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Solid.hpp"

struct Face
{
    glm::vec3 p0, p1, p2;
};

class Mesh : public Solid
{
private:
    std::vector<Face> _faces;
    glm::vec3 _center;

public:
    Mesh();
    /* TODO: Implement
    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);
    */
    Mesh(std::vector<Face> faces, glm::vec3 center, Material material);
    std::optional<RayHit> intersect(const Ray& ray) override;
    glm::vec3 calculateNormal(glm::vec3 point) const override;
    glm::vec3 getCenter() const;
    std::vector<Face> getFaces() const;
};
