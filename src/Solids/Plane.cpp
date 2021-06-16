#include "Plane.hpp"

Plane::Plane(glm::vec3 center, glm::vec3 normal, Material material) :
	_center(center),
	_normal(normal),
	Solid(material)
{
}

std::optional<RayHit> Plane::intersect(const Ray& ray)
{
	auto D = (-(_normal.x * _center.x) - (_normal.y * _center.y) - (_normal.z * _center.z)); // - Producto interno normal y centro

	auto denominator = (_normal.x * ray.direction.x) + (_normal.y * ray.direction.y) + (_normal.z * ray.direction.z); // Producto interno normal y direccion
	auto numerator = -D - (_normal.x * ray.origin.x) - (_normal.y * ray.origin.y) - (_normal.z * ray.origin.z); // - D - Producto interno normal y origen

	auto t = numerator / denominator;
	auto intersection = ray.origin + t * ray.direction;
	if (t > 0.f)
	{
		return RayHit{ intersection, _normal, shared_from_this(), t };
	}
	else
	{
		return {};
	}
}

glm::vec3 Plane::calculateNormal(glm::vec3 point) const
{
	return _normal;
}
