#pragma once
#include <vector>
#include "SceneObject.h"
#include <set>
#include <Camera.h>
#include <iterator>
#include "Light.h"
#include "ImageStorage.h"
class Scene
{
	std::vector<SceneObject *> sceneObjects;
	std::vector<Light*> lights;
	Camera * mainCamera;
	ImageStorage imageStorage;
public:
	void addSceneObject(SceneObject * sceneObject);
	void AddLight(Light light);
	std::vector<SceneObject *>& GetSceneObjects();
	const std::vector<SceneObject *>& GetSceneObjects() const;
	std::vector<Light*>& GetLights();
	const std::vector<Light*>& GetLights() const;
	void SetMainCamera(Camera& camera);
	Camera& getMainCamera();
	const Camera& getMainCamera() const;
	ImageStorage& getImageStorage();
	const ImageStorage& getImageStorage() const;
	~Scene();
};

