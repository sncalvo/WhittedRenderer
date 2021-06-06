#include <iostream>

#include "Image.hpp"
#include "Sphere.hpp"

int main(void)
{
    Image image(1080, 1920);

    // Solids
    Sphere sphere(glm::vec3(0.f, 0.f, 1.5f), .4f);

    bool allCollide = true;

    for (unsigned int row = 0; row < image.getHeight(); ++row)
    {
        for (unsigned int column = 0; column < image.getWidth(); ++column)
        {
            // Window goes from -1 to 1 in both axis
            float y = (row / (image.getHeight() - 1.f)) * 2 - 1;
            float x = (column / (image.getWidth() - 1.f)) * 2 - 1;

            Ray ray{ glm::vec3(0.f), glm::vec3(x, y, 1.f) };

            if (sphere.intersect(ray))
            {
                image[row * image.getWidth() + column] = { 0xFF, 0x0, 0x0 };
            }
            else
            {
                image[row * image.getWidth() + column] = { 0x0, 0x0, 0x0 };
            }
        }
    }

    image.write("test.png");
}
