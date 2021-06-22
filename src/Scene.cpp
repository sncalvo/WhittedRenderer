#include "Scene.hpp"

#include <iostream>
#include "Solids/Sphere.hpp"
#include "Solids/Cylinder.hpp"
#include "Solids/Disc.hpp"
#include "Solids/Solid.hpp"
#include "Solids/Plane.hpp"
#include "Solids/Mesh.hpp"
#include "Camera.hpp"

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Material> {
        static Node encode(const Material& rhs) {
            Node node;
            node["diffuseColor"] = rhs.diffuseColor;
            node["specularColor"] = rhs.specularColor;
            node["diffuse"] = rhs.diffuse;
            node["specular"] = rhs.specular;
            node["transparency"] = rhs.transparency;
            node["refractionIndex"] = rhs.refractionIndex;
            return node;
        }

        static bool decode(const Node& node, Material& rhs) {
            if (!node.IsMap() || node.size() != 6) {
                return false;
            }

            rhs.diffuseColor = node["diffuseColor"].as<glm::vec3>();
            rhs.specularColor = node["specularColor"].as<glm::vec3>();
            rhs.diffuse = node["diffuse"].as<float>();
            rhs.specular = node["specular"].as<float>();
            rhs.transparency = node["transparency"].as<float>();
            rhs.refractionIndex = node["refractionIndex"].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Sphere>
    {
        static Node encode(const Sphere& rhs)
        {
            Node node;
            node["type"] = "sphere";
            node["center"] = rhs.getCenter();
            node["radius"] = rhs.getRadius();
            node["material"] = rhs.getMaterial();
            return node;
        }

        static bool decode(const Node& node, Sphere& rhs)
        {
            // 4 because of the type field
            if (!node.IsMap() || node.size() != 4)
            {
                return false;
            }

            rhs = Sphere(
                node["center"].as<glm::vec3>(),
                node["radius"].as<float>(),
                node["material"].as<Material>()
            );
            return true;
        }
    };

    template<>
    struct convert<Cylinder>
    {
        static Node encode(const Cylinder& rhs)
        {
            Node node;
            node["type"] = "cylinder";
            node["center"] = rhs.getCenter();
            node["radius"] = rhs.getRadius();
            node["height"] = rhs.getHeight();
            node["material"] = rhs.getMaterial();
            return node;
        }

        static bool decode(const Node& node, Cylinder& rhs)
        {
            // 5 because of the type field
            if (!node.IsMap() || node.size() != 5)
            {
                return false;
            }

            rhs = Cylinder(
                node["center"].as<glm::vec3>(),
                node["radius"].as<float>(),
                node["height"].as<float>(),
                node["material"].as<Material>()
            );
            return true;
        }
    };

    template<>
    struct convert<Disc>
    {
        static Node encode(const Disc& rhs)
        {
            Node node;
            node["type"] = "disc";
            node["center"] = rhs.getCenter();
            node["normal"] = rhs.getNormal();
            node["radius"] = rhs.getRadius();
            node["material"] = rhs.getMaterial();
            return node;
        }

        static bool decode(const Node& node, Disc& rhs)
        {
            // 5 because of the type field
            if (!node.IsMap() || node.size() != 5)
            {
                return false;
            }

            rhs = Disc(
                node["center"].as<glm::vec3>(),
                node["normal"].as<glm::vec3>(),
                node["radius"].as<float>(),
                node["material"].as<Material>()
            );
            return true;
        }
    };

    template<>
    struct convert<Light>
    {
        static Node encode(const Light& rhs)
        {
            Node node;
            node["position"] = rhs.position;
            node["color"] = rhs.color;
            node["intensity"] = rhs.intensity;
            node["decay"] = rhs.decay;
            return node;
        }

        static bool decode(const Node& node, Light& rhs)
        {
            if (!node.IsMap() || node.size() != 4)
            {
                return false;
            }

            rhs.position = node["position"].as<glm::vec3>();
            rhs.color = node["color"].as<glm::vec3>();
            rhs.intensity = node["intensity"].as<float>();
            rhs.decay = node["decay"].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Plane>
    {
        static Node encode(const Plane& rhs)
        {
            Node node;
            node["type"] = "plane";
            node["center"] = rhs.getCenter();
            node["normal"] = rhs.getNormal();
            node["material"] = rhs.getMaterial();
            return node;
        }

        static bool decode(const Node& node, Plane& rhs)
        {
            if (!node.IsMap() || node.size() != 4)
            {
                return false;
            }

            rhs = Plane(
                node["center"].as<glm::vec3>(),
                node["normal"].as<glm::vec3>(),
                node["material"].as<Material>()
            );
            return true;
        }
    };

    template<>
    struct convert<Camera>
    {
        static bool decode(const Node& node, Camera& rhs)
        {
            if (!node.IsMap() || node.size() != 4)
            {
                return false;
            }

            rhs = Camera(
                node["aspectRatio"].as<float>(),
                node["vFov"].as<float>(),
                node["origin"].as<glm::vec3>(),
                node["lookAt"].as<glm::vec3>()
            );
            return true;
        }
    };

    template<>
    struct convert<Face> {
        static Node encode(const Face& rhs) {
            Node node;
            node.push_back(rhs.p0);
            node.push_back(rhs.p1);
            node.push_back(rhs.p2);
            return node;
        }

        static bool decode(const Node& node, Face& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.p0 = node[0].as<glm::vec3>();
            rhs.p1 = node[1].as<glm::vec3>();
            rhs.p2 = node[2].as<glm::vec3>();
            return true;
        }
    };

    template<>
    struct convert<Mesh> {
        static Node encode(const Mesh& rhs) {
            Node node;
            node["type"] = "mesh";
            node["center"] = rhs.getCenter();
            node["material"] = rhs.getMaterial();
            node["faces"] = rhs.getFaces();
            return node;
        }

        static bool decode(const Node& node, Mesh& rhs) {
            /* Nodes are:
                - type
                - center
                - material
                - vertices
                - faces
            */
            if (!node.IsMap() || node.size() != 5) {
                return false;
            }

            rhs = Mesh(
                node["faces"].as<std::vector<Face>>(),
                node["center"].as<glm::vec3>(),
                node["material"].as<Material>()
            );
            return true;
        }
    };

}

std::vector<std::shared_ptr<Solid>> _loadSolids(YAML::Node solids)
{
    std::vector<std::shared_ptr<Solid>> result;
    for (std::size_t i = 0; i < solids.size(); i++) {
        if (solids[i]["type"].as<std::string>() == "sphere")
        {
            result.push_back(std::make_shared<Sphere>(solids[i].as<Sphere>()));
        }
        else if (solids[i]["type"].as<std::string>() == "cylinder")
        {
            result.push_back(std::make_shared<Cylinder>(solids[i].as<Cylinder>()));
        }
        else if (solids[i]["type"].as<std::string>() == "disc")
        {
            result.push_back(std::make_shared<Disc>(solids[i].as<Disc>()));
        }
        else if (solids[i]["type"].as<std::string>() == "plane")
        {
            result.push_back(std::make_shared<Plane>(solids[i].as<Plane>()));
        }
        else if (solids[i]["type"].as<std::string>() == "mesh")
        {
            result.push_back(std::make_shared<Mesh>(solids[i].as<Mesh>()));
        }
    }
    return result;
}

std::vector<std::shared_ptr<Light>> _loadLights(YAML::Node lights)
{
    std::vector<std::shared_ptr<Light>> result;
    for (std::size_t i = 0; i < lights.size(); i++) {
        result.push_back(std::make_shared<Light>(lights[i].as<Light>()));
    }
    return result;
}

std::shared_ptr<Camera> Scene::_loadCamera(YAML::Node camera)
{
    camera["aspectRatio"] = this->getAspectRatio();
    return std::make_shared<Camera>(camera.as<Camera>());
}

Scene::Scene(std::string fileLocation)
{
    _file = YAML::LoadFile("scene.yaml");
    if (!_file["width"] || !_file["height"] || !_file["solids"] || !_file["lights"])
    {
        return;
    }
    _solids = _loadSolids(_file["solids"]);
    _lights = _loadLights(_file["lights"]);
    _camera = _loadCamera(_file["camera"]);
}

int Scene::getWidth() const
{
    return _file["width"].as<int>();
}

int Scene::getHeight() const
{
    return _file["height"].as<int>();
}

int Scene::getSamples() const
{
    return _file["samples"].as<int>();
}

std::vector<std::shared_ptr<Solid>> Scene::getSolids() const
{
    return _solids;
}

std::vector<std::shared_ptr<Light>> Scene::getLights() const
{
    return _lights;
}

float Scene::getAspectRatio() const
{
    return (float) this->getWidth() / (float) this->getHeight();
}

std::shared_ptr<Camera> Scene::getCamera() const
{
    return _camera;
}
