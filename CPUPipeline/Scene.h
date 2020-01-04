#pragma once
#include <vector>
#include "SceneObject.h"
#include <set>
#include <Camera.h>
#include <iterator>
class Scene
{
	std::vector<SceneObject *> sceneObjects;
	Camera * mainCamera;
public:
	void AddSceneObject(SceneObject & sceneObject);
	std::vector<SceneObject *>& GetSceneObjects();
	const std::vector<SceneObject *>& GetSceneObjects() const;
	void SetMainCamera(Camera& camera);
	Camera& getMainCamera();
	const Camera& getMainCamera() const;
};

