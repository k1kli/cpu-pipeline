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

void Scene::load(SceneDataReader& reader)
{
	cleanup();
	imageStorage.load(reader);
	int sceneObjectsCount = reader.readInt();
	sceneObjects.resize(sceneObjectsCount);
	for (int i = 0; i < sceneObjectsCount; i++)
	{
		sceneObjects[i] = new SceneObject(reader);
	}
	int lightsCount = reader.readInt();
	lights.resize(lightsCount);
	for (int i = 0; i < lightsCount; i++)
	{
		lights[i] = new Light(reader);
	}
	int cameraCount = reader.readInt();
	cameras.resize(cameraCount);
	for (int i = 0; i < cameraCount; i++)
	{
		cameras[i] = new Camera(reader);
	}
	int mainCameraId = reader.readInt();
	SetMainCamera(mainCameraId);
}

void Scene::save(SceneDataWriter& writer) const
{
	imageStorage.save(writer);
	writer.write((int)sceneObjects.size());
	for (SceneObject* so : sceneObjects)
	{
		so->save(writer);
	}
	writer.write((int)lights.size());
	for (Light* light : lights)
	{
		light->save(writer);
	}
	writer.write((int)cameras.size());
	int mainCameraId = 0;
	for (int i = 0; i < cameras.size(); i++)
	{
		if (mainCamera == cameras[i]) mainCameraId = i;
		cameras[i]->save(writer);
	}
	writer.write(mainCameraId);
}

void Scene::cleanup()
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
	sceneObjects.resize(0);
	lights.resize(0);
	cameras.resize(0);
}

Scene::~Scene()
{
	cleanup();
}
