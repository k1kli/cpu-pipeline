#include "Scene.h"

void Scene::addSceneObject(SceneObject * sceneObject)
{
	sceneObjects.push_back(sceneObject);
}

void Scene::AddLight(Light light)
{
	lights.push_back(new Light(light));
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

ImageStorage& Scene::getImageStorage()
{
	return imageStorage;
}

const ImageStorage& Scene::getImageStorage() const
{
	return imageStorage;
}

Scene::~Scene()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
	}
	for (int i = 0; i < lights.size(); i++)
	{
		delete lights[i];
	}
}
