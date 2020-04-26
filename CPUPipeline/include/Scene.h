#pragma once
#include <vector>
#include "SceneObject.h"
#include <set>
#include <Camera.h>
#include <iterator>
#include "Light.h"
#include "ImageStorage.h"
#include "SaveableObject.h"
class Scene : public SaveableObject
{
	std::vector<SceneObject *> sceneObjects;
	std::vector<Light*> lights;
	Camera * mainCamera = nullptr;
	ImageStorage imageStorage;
	std::vector<Camera*> cameras;
public:
	void addSceneObject(SceneObject * sceneObject);
	void AddLight(Light light);
	void AddCamera(Camera camera);
	std::vector<SceneObject *>& GetSceneObjects();
	const std::vector<SceneObject *>& GetSceneObjects() const;
	std::vector<Light*>& GetLights();
	const std::vector<Light*>& GetLights() const;
	void SetMainCamera(int cameraIndex);
	Camera& getMainCamera();
	const Camera& getMainCamera() const;
	std::vector<Camera*>& getCameras();
	const std::vector<Camera*>& getCameras() const;
	ImageStorage& getImageStorage();
	const ImageStorage& getImageStorage() const;
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;
	void cleanup();
	~Scene();
};

