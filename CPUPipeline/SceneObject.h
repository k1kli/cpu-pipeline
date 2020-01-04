#pragma once
#include "Mesh.h"
class SceneObject
{
	Mesh mesh;
	glm::mat4 worldMatrix;
public:
	Mesh& GetMesh();
	const Mesh& GetMesh() const;
	const glm::mat4& GetWorldMatrix() const;
	void SetWorldMatrix(const glm::mat4& worldMatrix);
};

