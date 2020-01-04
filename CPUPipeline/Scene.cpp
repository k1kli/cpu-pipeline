#include "Scene.h"

void Scene::AddSceneObject(SceneObject& sceneObject)
{
	sceneObjects.push_back(&sceneObject);
}


std::vector<SceneObject *>& Scene::GetSceneObjects()
{
	return sceneObjects;
}

const std::vector<SceneObject *>& Scene::GetSceneObjects() const
{
	return sceneObjects;
}

void Scene::SetMainCamera(Camera& camera)
{
	mainCamera = &camera;
}

Camera& Scene::getMainCamera()
{
	return *mainCamera;
}

const Camera& Scene::getMainCamera() const
{
	return *mainCamera;
}
