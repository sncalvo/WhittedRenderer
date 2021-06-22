#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>

#include <random>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <yaml-cpp/yaml.h>

#include "Camera.hpp"
#include "Image.hpp"
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"
#include "Solids/Mesh.hpp"
#include "Solids/Plane.hpp"
#include "LoadingBar.hpp"
#include "Scene.hpp"
#include "windows.h"

// TODO: Use
constexpr auto THREADS = 8;
constexpr auto THREAD_LOAD = 10;

int main(void)
{
    auto scene = Scene();
    auto solids = scene.getSolids();

    auto width = scene.getWidth();
    auto height = scene.getHeight();
    auto samples = scene.getSamples();

    Image image(width, height);
    Image reflectionImage(width, height);
    Image transparencyImage(width, height);

    LoadingBar loading("Render started", scene.getHeight());

    auto camera = scene.getCamera();

    auto start = std::chrono::steady_clock::now();

    auto maxColor = glm::vec3(0.f);
    for (auto row = image.getHeight(); row > 0; --row)
    {
        for (auto column = 0; column < image.getWidth(); ++column)
        {
            auto color = glm::vec3(0.f);
            auto material = Material();
            auto rowIndex = (image.getHeight() - row);

            for (auto sample = 0; sample < samples; ++sample)
            {
                auto randomU = 0.f;
                auto randomV = 0.f;

                if (samples > 1)
                {
                    randomU = glm::linearRand(0.0, 1.0);
                    randomV = glm::linearRand(0.0, 1.0);
                }

                float u = (column + randomU) / (image.getWidth() - 1);
                float v = (row + randomV) / (image.getHeight() - 1);

                auto ray = camera->createRay(u, v);

                std::tie(color, material) = ray.calculateColorAndMaterial(solids, 0, scene);
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

    image.write(fileNameStr.c_str(), maxColor, samples);
    reflectionImage.write(reflectionFileNameStr.c_str(), glm::vec3(1.f), 1);
    transparencyImage.write(transparencyFileNameStr.c_str(), glm::vec3(1.f), 1);

    std::cout << std::endl << "Image saved at " << fileNameStr << std::endl;
    std::cout << "Opening " << fileNameStr << std::endl;
    ShellExecute(0, 0, std::wstring(fileNameStr.begin(), fileNameStr.end()).c_str(), 0, 0, SW_SHOW);
}
