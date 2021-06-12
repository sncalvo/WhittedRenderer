#include <iostream>
#include <vector>
#include <glm/gtc/random.hpp>

#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"

constexpr auto SAMPLES = 10;

constexpr auto THREADS = 8;
constexpr auto THREAD_LOAD = 10;

int main(void)
{
    Image image(768, 1366);

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
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            Pixel pixel{0x0, 0x0, 0x0};
            auto rowIndex = (image.getHeight() - row);

            for (auto sample = 0; sample < SAMPLES; ++sample)
            {
                float nearestIntersection = UINT32_MAX;

                float y = ((row + glm::gaussRand(0.f, 1.f)) / (image.getHeight() - 1.f)) * 2 - 1;
                float x = ((column + glm::gaussRand(0.f, 1.f)) / (image.getHeight() - 1.f)) * 2 - image.aspectRatio();

                Ray ray{ glm::vec3(0.f), glm::vec3(x, y, focalLength) };

                pixel += ray.calculateColor(solids, 0) / SAMPLES;
            }

            image[rowIndex * image.getWidth() + column] = pixel;
        }
    }

    image.write("test.png");
}
