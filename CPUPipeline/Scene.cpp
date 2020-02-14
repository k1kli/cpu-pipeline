#include "Scene.h"

void Scene::addSceneObject(SceneObject * sceneObject)
{
	sceneObjects.push_back(sceneObject);
}

void Scene::AddLight(Light light)
{
	lights.push_back(new Light(light));
}

void Scene::AddCamera(Camera camera)
{
	cameras.push_back(new Camera(camera));
	if (mainCamera == nullptr)
		mainCamera = cameras[cameras.size()-1];
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

void Scene::SetMainCamera(int cameraIndex)
{
	if (cameras.size() <= cameraIndex || cameraIndex < 0) return;
	mainCamera = cameras[cameraIndex];
}

Camera& Scene::getMainCamera()
{
	return *mainCamera;
}

const Camera& Scene::getMainCamera() const
{
	return *mainCamera;
}

std::vector<Camera*>& Scene::getCameras()
{
	return cameras;
}

const std::vector<Camera*>& Scene::getCameras() const
{
	return cameras;
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
	for (int i = 0; i < cameras.size(); i++)
	{
		delete cameras[i];
	}
}
