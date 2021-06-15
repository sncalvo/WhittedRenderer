#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>

#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

#include "Camera.hpp"
#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"
#include "LoadingBar.hpp"

#include "windows.h"
#include "Solids/Plane.hpp"

constexpr auto SAMPLES = 4;

// TODO: Use
constexpr auto THREADS = 8;
constexpr auto THREAD_LOAD = 10;

int main(void)
{
    int width = 1366;
    int height = 768;
    LoadingBar loading("Render started", height);
    Image image(width, height);

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
        .0f,
        1.f,
        2.f
    };
    Material material3{
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(1.f, 1.f, 1.f),
        1.f,
        1.f,
        0.f,
        0.f
    };

    // Solids
    std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(
        glm::vec3(1.5f, 0.f, 9.f),
        1.f,
        material
    );
    std::shared_ptr<Cylinder> cylinder = std::make_shared<Cylinder>(
        glm::vec3(0.f, 0.f, 5.f),
        1.f,
        2.f,
        material2
    );
    std::shared_ptr<Plane> plane = std::make_shared<Plane>(
        glm::vec3(70.f, 0.f, 0.f),
        glm::vec3(-1.f, 0.f, 0.f),
        material
    );
    std::shared_ptr<Plane> plane2 = std::make_shared<Plane>(
        glm::vec3(-70.f, 0.f, 0.f),
        glm::vec3(1.f, 0.f, 0.f),
        material2
    );
    std::shared_ptr<Plane> plane3 = std::make_shared<Plane>(
        glm::vec3(0.f, 0.f, 20.f),
        glm::vec3(0.f, 0.f, -1.f),
        material3
    );

    std::vector<std::shared_ptr<Solid>> solids;
    solids.push_back(std::move(sphere));
    solids.push_back(std::move(plane));
    solids.push_back(std::move(plane2));
    solids.push_back(std::move(plane3));
    solids.push_back(std::move(cylinder));

    // Camera
    auto vFov = 45.f;
    auto origin = glm::vec3(0.f, 5.f, 0.f);
    auto lookAt = glm::vec3(0.f, 0.f, 5.f);
    Camera camera(image.aspectRatio(), vFov, origin, lookAt);

    auto start = std::chrono::steady_clock::now();

    auto maxColor = glm::vec3(0.f);
    for (auto row = image.getHeight(); row > 0; --row)
    {
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            auto color = glm::vec3(0.f);
            auto rowIndex = (image.getHeight() - row);

            for (auto sample = 0; sample < SAMPLES; ++sample)
            {
                float u = (column + glm::linearRand(0.0, 1.0)) / (image.getWidth() - 1);
                float v = (row + glm::linearRand(0.0, 1.0)) / (image.getHeight() - 1);

                auto ray = camera.createRay(u, v);

                color += ray.calculateColor(solids, 0);
                if (glm::l2Norm(color) > glm::l2Norm(maxColor))
                {
                    maxColor = color;
                }
            }

            image[rowIndex * image.getWidth() + column] = color;
        }
        loading.incrementProgress(1);
        loading.draw("Rendering");
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    auto now = time(nullptr);

    std::stringstream fileName;
    fileName << "images\\test-" << now << ".png";
    std::cout << std::endl << "Ray tracing took " << duration / 1000.0 << " seconds" << std::endl;
    std::cout << "Saving...";
    auto fileNameStr = fileName.str();

    image.write(fileNameStr.c_str(), maxColor, SAMPLES);

    std::cout << std::endl << "Image saved at " << fileNameStr << std::endl;
    std::cout << "Opening " << fileNameStr << std::endl;
    ShellExecute(0, 0, std::wstring(fileNameStr.begin(), fileNameStr.end()).c_str(), 0, 0, SW_SHOW);
}
