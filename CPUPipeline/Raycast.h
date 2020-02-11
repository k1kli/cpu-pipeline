#pragma once
#include "Scene.h"
class Raycast
{
public:
	Raycast(Scene& scene) :scene(scene) {}
	SceneObject& castRay(glm::vec3 from, glm::vec3 direction);
private:
	Scene& scene;
	glm::vec3 from = { 0,0,0 };
	glm::vec3 direction = { 0,0,0 };
	const std::vector<glm::uvec3>* triangles;
	std::vector<glm::vec3> vertices;
	SceneObject* sceneObject;

	void prepareTriangles();
	bool checkTriangles();
	bool intersectPlane(const glm::vec3& planeNormal, const glm::vec3& p0, float* tRes);
	bool insideTriangle(const glm::vec3& point, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
};

