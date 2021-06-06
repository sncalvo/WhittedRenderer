#include <iostream>

#include "Image.hpp"
#include "Solids/Sphere.hpp"

int main(void)
{
    Image image(1080, 1920);

    // Solids
    Sphere sphere(glm::vec3(0.f, 0.f, 1.5f), .4f);

    const auto focalLength = 1.f;

    for (auto row = image.getHeight(); row > 0; --row)
    {
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            float y = (row / (image.getHeight() - 1.f)) * 2 - 1;
            float x = (column / (image.getHeight() - 1.f)) * 2 - image.aspectRatio();

            Ray ray{ glm::vec3(0.f), glm::vec3(x, y, focalLength) };

            auto rowIndex = (image.getHeight() - row);
            if (sphere.intersect(ray))
            {
                image[rowIndex * image.getWidth() + column] = { 0xFF, 0x0, 0x0 };
            }
            else
            {
                image[rowIndex * image.getWidth() + column] = { 0x0, 0x0, 0x0 };
            }
        }
    }

    image.write("test.png");
}
