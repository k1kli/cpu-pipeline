#include "SceneObject.h"

Mesh& SceneObject::GetMesh()
{
	return mesh;
}

const Mesh& SceneObject::GetMesh() const
{
	return mesh;
}

const glm::mat4& SceneObject::GetWorldMatrix() const
{
	return worldMatrix;
}

void SceneObject::SetWorldMatrix(const glm::mat4& worldMatrix)
{
	this->worldMatrix = worldMatrix;
}
