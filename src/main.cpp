#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>

#include <random>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

#include "Camera.hpp"
#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"
#include "Solids/Mesh.hpp"
#include "Solids/Plane.hpp"
#include "LoadingBar.hpp"

#include "windows.h"

constexpr auto SAMPLES = 1;

// TODO: Use
constexpr auto THREADS = 8;
constexpr auto THREAD_LOAD = 10;

int main(void)
{
    int width = 640;
    int height = 360;
    LoadingBar loading("Render started", height);
    Image image(width, height);
    Image reflectionImage(width, height);
    Image transparencyImage(width, height);

    Material white(
        glm::vec3(1.f),
        glm::vec3(1.f),
        1.f,
        0.f,
        0.f,
        0.f
    );

    // Solids
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
        std::vector<Face>{
        Face{ glm::vec3{0.f, 0.f, 5.f}, glm::vec3{-1.f, 1.f, 6.f}, glm::vec3{1.f, 1.f, 6.f} },
            Face{ glm::vec3{-1.f, 1.f, 6.f}, glm::vec3{0.f, 0.f, 5.f}, glm::vec3{-1.f, 0.f, 6.f} },
            Face{ glm::vec3{-1.f, 1.f, 6.f}, glm::vec3{0.f, 0.f, 5.f}, glm::vec3{-1.f, -1.f, 6.f} },
            Face{ glm::vec3{-1.f, -1.f, 6.f}, glm::vec3{0.f, 0.f, 5.f}, glm::vec3{1.f, -1.f, 6.f} }
        },
        glm::vec3(-1.f),
        white
    );
    std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(
        glm::vec3(1.f, -1.f, 4.f),
        .5f,
        Material(
            glm::vec3(1.f),
            glm::vec3(1.f),
            1.f,
            1.f,
            0.f,
            0.f
        )
    );
    std::shared_ptr<Cylinder> cylinder = std::make_shared<Cylinder>(
        glm::vec3(0.f, 0.f, 5.f),
        1.f,
        2.f,
        Material(
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.f),
            1.f,
            0.f,
            0.f,
            0.f
        )
    );
    std::shared_ptr<Plane> rightWall = std::make_shared<Plane>(
        glm::vec3(2.f, 0.f, 0.f),
        glm::vec3(-1.f, 0.f, 0.f),
        Material(
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.f, 1.f, 1.f),
            1.f,
            0.f,
            0.f,
            0.f
        )
    );
    std::shared_ptr<Plane> leftWall = std::make_shared<Plane>(
        glm::vec3(-2.f, 0.f, 0.f),
        glm::vec3(1.f, 0.f, 0.f),
        Material(
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(1.f, 1.f, 1.f),
            1.f,
            0.f,
            0.f,
            0.f
        )
    );
    std::shared_ptr<Plane> frontWall = std::make_shared<Plane>(
        glm::vec3(0.f, 0.f, 6.f),
        glm::vec3(0.f, 0.f, -1.f),
        white
    );
    std::shared_ptr<Plane> backWall = std::make_shared<Plane>(
        glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(0.f, 0.f, 1.f),
        Material(
            glm::vec3(0.f),
            glm::vec3(1.f),
            1.f,
            0.f,
            0.f,
            0.f
        )
    );
    std::shared_ptr<Plane> topWall = std::make_shared<Plane>(
        glm::vec3(0.f, 2.f, 0.f),
        glm::vec3(0.f, -1.f, 0.f),
        Material(
            glm::vec3(0.5f),
            glm::vec3(1.f),
            1.f,
            0.f,
            0.f,
            0.f
        )
    );
    std::shared_ptr<Plane> bottomWall = std::make_shared<Plane>(
        glm::vec3(0.f, -2.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        white
    );

    std::vector<std::shared_ptr<Solid>> solids;
    solids.push_back(std::move(sphere));
    solids.push_back(std::move(rightWall));
    solids.push_back(std::move(leftWall));
    solids.push_back(std::move(frontWall));
    solids.push_back(std::move(backWall));
    solids.push_back(std::move(topWall));
    solids.push_back(std::move(bottomWall));
    solids.push_back(std::move(mesh));
    //solids.push_back(std::move(cylinder));

    // Camera
    auto vFov = 45.f;
    auto origin = glm::vec3(0.f, 0.f, 0.f);
    auto lookAt = glm::vec3(0.f, 0.f, 5.f);
    Camera camera(image.aspectRatio(), vFov, origin, lookAt);

    auto start = std::chrono::steady_clock::now();

    auto maxColor = glm::vec3(0.f);
    for (auto row = image.getHeight(); row > 0; --row)
    {
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            auto color = glm::vec3(0.f);
            auto material = Material();
            auto rowIndex = (image.getHeight() - row);

            for (auto sample = 0; sample < SAMPLES; ++sample)
            {
                float u = (column + glm::linearRand(0.0, 1.0)) / (image.getWidth() - 1);
                float v = (row + glm::linearRand(0.0, 1.0)) / (image.getHeight() - 1);

                auto ray = camera.createRay(u, v);

                std::tie(color, material) = ray.calculateColorAndMaterial(solids, 0);
                if (glm::l2Norm(color) > glm::l2Norm(maxColor))
                {
                    maxColor = color;
                }
            }

            auto reflectionColor = glm::vec3(material.specular);
            auto transparencyColor = glm::vec3(material.transparency);

            reflectionImage[(size_t)rowIndex * image.getWidth() + column] = reflectionColor;
            transparencyImage[(size_t)rowIndex * image.getWidth() + column] = transparencyColor;

            image[(size_t) rowIndex * image.getWidth() + column] = color;
        }
        loading.incrementProgress(1);
        loading.draw("Rendering");
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << std::endl << "Ray tracing took " << duration / 1000.0 << " seconds" << std::endl;
    std::cout << "Saving...";

    auto now = time(nullptr);

    std::stringstream fileName;
    fileName << "images\\test-" << now << ".png";
    auto fileNameStr = fileName.str();

    std::stringstream reflectionFileName, transparencyFileName;
    reflectionFileName << "images\\test-" << now << "-reflection.png";
    transparencyFileName << "images\\test-" << now << "-transparency.png";
    auto reflectionFileNameStr = reflectionFileName.str();
    auto transparencyFileNameStr = transparencyFileName.str();

    image.write(fileNameStr.c_str(), maxColor, SAMPLES);
    reflectionImage.write(reflectionFileNameStr.c_str(), glm::vec3(1.f), 1);
    transparencyImage.write(transparencyFileNameStr.c_str(), glm::vec3(1.f), 1);

    std::cout << std::endl << "Image saved at " << fileNameStr << std::endl;
    std::cout << "Opening " << fileNameStr << std::endl;
    ShellExecute(0, 0, std::wstring(fileNameStr.begin(), fileNameStr.end()).c_str(), 0, 0, SW_SHOW);
}
