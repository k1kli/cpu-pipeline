#pragma once
#include <vector>
#include "SceneObject.h"
#include <set>
#include <Camera.h>
#include <iterator>
#include "Light.h"
class Scene
{
	std::vector<SceneObject *> sceneObjects;
	std::vector<Light*> lights;
	Camera * mainCamera;
public:
	void addSceneObject(SceneObject * sceneObject);
	void AddLight(Light& light);
	std::vector<SceneObject *>& GetSceneObjects();
	const std::vector<SceneObject *>& GetSceneObjects() const;
	std::vector<Light*>& GetLights();
	const std::vector<Light*>& GetLights() const;
	void SetMainCamera(Camera& camera);
	Camera& getMainCamera();
	const Camera& getMainCamera() const;
	~Scene();
};

