#include "Raycast.h"

SceneObject* Raycast::castRay(glm::vec3 from, glm::vec3 direction)
{
	this->from = from;
	this->direction = glm::normalize(direction);
	std::vector<SceneObject*>& sceneObjects = scene.GetSceneObjects();
	SceneObject* closest = nullptr;
	float closestDist = INFINITY;
	for (int i = 0; i < (int)sceneObjects.size(); i++)
	{
		sceneObject = sceneObjects[i];
		prepareTriangles();
		float dist;
		if (checkTriangles(&dist) && dist < closestDist)
		{
			closest = sceneObject;
			closestDist = dist;
		}
	}
	return closest;
}

void Raycast::prepareTriangles()
{
	triangles = &(sceneObject->GetMesh().getTriangles());
	vertices = sceneObject->GetMesh().getVertices();
	glm::mat4 worldMatrix = sceneObject->GetTransform().GetWorldMatrix();
	for (int i = 0; i < (int)vertices.size(); i++)
	{
		glm::vec4 v = worldMatrix * glm::vec4(vertices[i], 1);
		vertices[i] = v/v.w;
	}
}

bool Raycast::checkTriangles(float *distRes)
{

	for (const glm::uvec3& triangle : *triangles)
	{
		const glm::vec3& p0 = vertices[triangle.x];
		const glm::vec3& p1 = vertices[triangle.y];
		const glm::vec3& p2 = vertices[triangle.z];
		if (rayIntersectsTriangle(p0, p1, p2, distRes)) return true;
		
	}
	return false;
}

//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Raycast::rayIntersectsTriangle(
	const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2, float * tRes)
{
	constexpr float EPSILON = 1e-6f;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(direction, edge2);
	a = glm::dot(edge1,h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	f = 1.0f / a;
	s = from - vertex0;
	u = f * glm::dot(s,h);
	if (u < 0.0f || u > 1.0f)
		return false;
	q = glm::cross(s,edge1);
	v = f * glm::dot(direction,q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	*tRes = f * glm::dot(edge2,q);
	return *tRes > EPSILON;
}
