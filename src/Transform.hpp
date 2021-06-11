#pragma once

#include <glm/glm.hpp>

class Transform
{
private:
    glm::vec3 _up;
    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _rotation;
    void _updateVectors();
    glm::mat4 _getTransformationMatrix() const;
    glm::mat4 _getTransformationMatrixWithoutTranslation() const;
    glm::vec3 _quaternionToEuler(glm::quat) const;

public:
    const glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f);
    Transform();
    glm::vec3 position, scale;
    glm::vec3 getRotation() const;
    void setRotation(glm::vec3 rotation);
    void rotateX(float);
    void rotateY(float);
    void rotateZ(float);
    void setRotationX(float);
    void setRotationY(float);
    void setRotationZ(float);
    void lookAt(glm::vec3 direction);
    glm::vec3 getUp() const;
    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::mat4 getInverseTransformationMatrix() const;
    glm::mat4 getInverseTransformationMatrixWithoutTranslation() const;
};
