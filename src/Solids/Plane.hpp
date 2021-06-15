#pragma once

#include "Solid.hpp"

class Plane : public Solid
{
private:
	glm::vec3 _center;
	glm::vec3 _normal;
public:
	Plane(glm::vec3 center, glm::vec3 normal, Material material);
	std::optional<RayHit> intersect(const Ray& ray);
	glm::vec3 calculateNormal(glm::vec3 point) const;
};
