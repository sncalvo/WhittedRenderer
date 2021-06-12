#include <iostream>

#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"

#include <vector>;

int main(void)
{
    Image image(1080, 1920);

    Material material{
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(0.f, 0.f, 0.f),
        1.f,
        1.f,
        0.f,
    };

    Material material2{
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(0.f, 0.f, 0.f),
        1.f,
        1.f,
        0.f,
    };

    // Solids
    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(glm::vec3(2.5f, 0.f, 3.f), 1.f, material);
    std::unique_ptr<Cylinder> cylinder = std::make_unique<Cylinder>(glm::vec3(-1.5f, 0.f, 3.f), 1.5f, 2.f, material2);

    std::vector<std::unique_ptr<Solid>> solids;
    solids.push_back(std::move(sphere));
    solids.push_back(std::move(cylinder));

    const auto focalLength = 1.f;

    for (auto row = image.getHeight(); row > 0; --row)
    {
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            float nearestIntersection = UINT32_MAX;

            float y = (row / (image.getHeight() - 1.f)) * 2 - 1;
            float x = (column / (image.getHeight() - 1.f)) * 2 - image.aspectRatio();

            Ray ray { glm::vec3(0.f), glm::vec3(x, y, focalLength) };

            auto rowIndex = (image.getHeight() - row);

            image[rowIndex * image.getWidth() + column] = ray.calculateColor(solids, 0);
        }
    }

    image.write("test.png");
}
