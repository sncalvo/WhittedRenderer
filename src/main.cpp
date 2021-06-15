#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>
#include <glm/gtc/random.hpp>

#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"

constexpr auto SAMPLES = 1;

// TODO: Use
constexpr auto THREADS = 8;
constexpr auto THREAD_LOAD = 10;

int main(void)
{
    Image image(1280, 720);

    Material material{
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
        1.f,
        1.f,
        0.f,
        0.f
    };

    Material material2{
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
        1.f,
        1.f,
        0.f,
        0.f
    };

    // Solids
    std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(
        glm::vec3(0.f, 0.f, 5.f),
        1.f,
        material
    );
    std::shared_ptr<Cylinder> cylinder = std::make_shared<Cylinder>(
        glm::vec3(2.f, -2.f, 3.f),
        1.f,
        2.f,
        material2
    );

    std::vector<std::shared_ptr<Solid>> solids;
    solids.push_back(std::move(sphere));
    solids.push_back(std::move(cylinder));

    // Camera
    auto vFov = 45.f;
    auto theta = glm::radians(vFov);
    auto h = glm::tan(theta / 2);

    auto viewportHeight = 2.0 * h;
    auto viewportWidth = image.aspectRatio() * viewportHeight;

    auto focalLength = 1.f;

    auto origin = glm::vec3(0.f);
    auto horizontal = glm::vec3(viewportWidth, 0.f, 0.f);
    auto vertical = glm::vec3(0.f, viewportHeight, 0.f);
    auto lowerLeftCorner = origin + glm::vec3(0.f, 0.f, focalLength) - horizontal / 2.f - vertical / 2.f;

    auto start = std::chrono::steady_clock::now();

    for (auto row = image.getHeight(); row > 0; --row)
    {
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            Pixel pixel{0x00, 0x00, 0x00};
            auto rowIndex = (image.getHeight() - row);

            for (auto sample = 0; sample < SAMPLES; ++sample)
            {
                float nearestIntersection = UINT32_MAX;

                float randomU = 0.f;
                float randomV = 0.f;
                if (SAMPLES > 1) {
                    randomU = glm::gaussRand(0.f, 1.f);
                    randomV = glm::gaussRand(0.f, 1.f);
                }

                float u = (column + randomU) / (image.getWidth() - 1);
                float v = (row + randomV) / (image.getHeight() - 1);

                Ray ray{ origin, glm::normalize(lowerLeftCorner + u * horizontal + v * vertical - origin) };

                pixel += ray.calculateColor(solids, 0) / SAMPLES;
            }

            image[rowIndex * image.getWidth() + column] = pixel;
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\rRay tracing took " << duration / 1000.0 << " seconds" << std::endl;

    auto now = time(nullptr);

    std::stringstream fileName;
    fileName << "images/test-" << now << ".png";
    image.write(fileName.str().c_str());
}
