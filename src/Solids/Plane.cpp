#include "Plane.hpp"

Plane::Plane(glm::vec3 center, glm::vec3 normal, Material material) :
	_center(center),
	_normal(normal),
	Solid(material)
{
}

std::optional<RayHit> Plane::intersect(const Ray& ray)
{
	
	
	/*auto D = (-(_normal.x * _center.x) - (_normal.y * _center.y) - (_normal.z * _center.z));

	auto denominator = (_normal.x * ray.direction.x) + (_normal.y * ray.direction.y) + (_normal.z * ray.direction.z);
	auto numerator = -D - (_normal.x * ray.origin.x) - (_normal.y * ray.origin.y) - (_normal.z * ray.origin.z);

	auto t = numerator / denominator;
	auto intersection = ray.origin + t * ray.direction;
	if (t > 0.f)
	{
		return RayHit{ intersection, _normal, shared_from_this(), t };
	}
	else
	{
		return {};
	}*/
	
	// Plane
	auto A = _normal.x;
	auto B = _normal.y;
	auto C = _normal.z;

	auto x0 = _center.x;
	auto y0 = _center.y;
	auto z0 = _center.z;

	// Ray
	auto alpha0 = ray.origin.x;
	auto beta0 = ray.origin.y;
	auto gamma0 = ray.origin.z;

	auto alpha1 = ray.direction.x;
	auto beta1 = ray.direction.y;
	auto gamma1 = ray.direction.z;

	auto denominator = (A * alpha1) + (B * beta1) + (C * gamma1);

	if (denominator == 0.f)
	{
		return {};
	}

	auto numerator = -(A * alpha0) + (x0 * A) - (B * beta0) + (B * y0) - (C * gamma0) + (C * z0);
	auto t = numerator / denominator;

	auto intersection = ray.origin + t * ray.direction;
	if (t > 0.f && ((A * (intersection.x - x0)) + (B * (intersection.y - y0)) + (C * (intersection.z - z0))) == 0.f)
	{
		return RayHit{ intersection, _normal, shared_from_this(), t };
	}
	else
	{
		return {};
	}

	/*auto t = glm::dot(_normal, (_center - ray.origin)) / glm::dot(_normal, ray.direction);

	if (t >= 0)
	{
		auto intersectionPoint = ray.origin + t * ray.direction;
		return RayHit{ glm::vec3{0.f}, _normal, shared_from_this(), t };
	}
	
	return {};*/
}

glm::vec3 Plane::calculateNormal(glm::vec3 point) const
{
	return _normal;
}
