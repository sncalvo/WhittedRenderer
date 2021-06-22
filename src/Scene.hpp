#pragma once

#include "Light.hpp"

#include <yaml-cpp/yaml.h>

class Camera;
class Solid;

class Scene
{
private:
	YAML::Node _file;
	std::vector<std::shared_ptr<Solid>> _solids;
	std::vector<std::shared_ptr<Light>> _lights;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Camera> _loadCamera(YAML::Node camera);
public:
	Scene(std::string fileLocation = "scene.yaml");
	int getWidth() const;
	int getHeight() const;
	int getSamples() const;
	float getAspectRatio() const;
	std::vector<std::shared_ptr<Solid>> getSolids() const;
	std::vector<std::shared_ptr<Light>> getLights() const;
	std::shared_ptr<Camera> getCamera() const;
};
