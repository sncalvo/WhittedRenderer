#include "Cone.hpp"

#include "../math.hpp"

bool Cone::intersect(Ray ray)
{
    auto transformedRayOrigin = glm::vec3(
        transform.getInverseTransformationMatrix() * glm::vec4(ray.origin, 1.f)
    );
    auto transformedRayDirection = glm::vec3(
        transform.getInverseTransformationMatrixWithoutTranslation() *
            glm::vec4(ray.direction, 1.f)
    );

    auto a = (
        math::square(transformedRayDirection.x) +
            math::square(transformedRayDirection.z) -
            math::square(transformedRayDirection.y)
    );
    auto b = (
        2 * transformedRayOrigin.x * transformedRayDirection.x +
            2 * transformedRayOrigin.z * transformedRayDirection.z -
            2 * transformedRayOrigin.y * transformedRayDirection.y
    );
    auto c = (
        math::square(transformedRayOrigin.x) +
            math::square(transformedRayOrigin.z) -
            math::square(transformedRayOrigin.y)
    );
    
    auto t = math::solve(a, b, c);
    if (t < 0)
    {
        return false;
    }

    auto y = ray.origin.y + t * ray.direction.y;

    return true;
}