#include <iostream>

#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"

#include <vector>;

int main(void)
{
    Image image(1080, 1920);

    // Solids
    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(glm::vec3(0.f, 0.f, 2.f), .4f);
    std::unique_ptr<Cylinder> cylinder = std::make_unique<Cylinder>(glm::vec3(0.f, 0.f, 2.f), .4f, .5f);

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

            for (auto& solid : solids)
            {
                if (auto intersection = solid->intersect(ray); intersection.has_value() && nearestIntersection >= intersection->position.z)
                {
                    nearestIntersection = intersection->position.z;
                    image[rowIndex * image.getWidth() + column] = intersection->pixel;
                }
            }
        }
    }

    image.write("test.png");
}
