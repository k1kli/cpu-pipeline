#include "Raycast.h"

SceneObject& Raycast::castRay(glm::vec3 from, glm::vec3 direction)
{
	this->from = from;
	this->direction = direction;
	std::vector<SceneObject*>& sceneObjects = scene.GetSceneObjects();
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObject = sceneObjects[i];
		prepareTriangles();
		if (checkTriangles())
		{
			return *sceneObject;
		}
	}
}

void Raycast::prepareTriangles()
{
	triangles = &(sceneObject->GetMesh().getTriangles());
	vertices = sceneObject->GetMesh().getVertices();
	glm::mat4 worldMatrix = sceneObject->GetTransform().GetWorldMatrix();
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = worldMatrix * glm::vec4(vertices[i], 1);
	}
}

bool Raycast::checkTriangles()
{

	for (const glm::uvec3& triangle : *triangles)
	{
		const glm::vec3& p0 = vertices[triangle.x];
		const glm::vec3& p1 = vertices[triangle.x];
		const glm::vec3& p2 = vertices[triangle.x];
		glm::vec3 planeNormal = glm::cross(p1 - p0, p2 - p0);
		float t;
		if (!intersectPlane(planeNormal, p0, &t)) continue;
		glm::vec3 P = from + t * direction;
		if (insideTriangle(P, p0, p1, p2)) return true;
	}
	return false;
}

bool Raycast::intersectPlane(const glm::vec3 & planeNormal, const glm::vec3 & p0, float * tRes)
{
	float denom = glm::dot(planeNormal, direction);
	if (denom > 1e-6)
	{
		glm::vec3 p0l0 = p0 - from;
		*tRes = glm::dot(p0l0, planeNormal);
		return (*tRes >= 0);
	}
	return false;
}

bool Raycast::insideTriangle(const glm::vec3& point, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	if (glm::dot(p1 - p0, point - p0) < 0) return false;
	if (glm::dot(p2 - p1, point - p1) < 0) return false;
	if (glm::dot(p0 - p2, point - p2) < 0) return false;
	return true;
}
