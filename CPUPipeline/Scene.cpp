#include "Scene.h"

void Scene::AddSceneObject(SceneObject& sceneObject)
{
	sceneObjects.push_back(&sceneObject);
}

void Scene::AddLight(Light& light)
{
	lights.push_back(&light);
}


std::vector<SceneObject *>& Scene::GetSceneObjects()
{
	return sceneObjects;
}

const std::vector<SceneObject *>& Scene::GetSceneObjects() const
{
	return sceneObjects;
}

std::vector<Light*>& Scene::GetLights()
{
	return lights;
}

const std::vector<Light*>& Scene::GetLights() const
{
	return lights;
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
