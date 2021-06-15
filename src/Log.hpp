#pragma once

#include <iostream>

template <class T>
void LOG(T var)
{
    std::cout << var << std::endl;
}

void LogVectors(glm::vec3 vector1, glm::vec3 vector2)
{
    std::cout << "(" << vector1.x << ", " << vector1.y << ", " << vector1.z << ") (" << vector2.x << ", " << vector2.y << ", "<< vector2.z << ")" << std::endl;
}

void LogVector(glm::vec3 vector)
{
    std::cout << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
}

void LogColor(glm::vec3 vector)
{
    std::cout << "R: " << vector.x << " G: " << vector.y << " B: " << vector.z << std::endl;
}
