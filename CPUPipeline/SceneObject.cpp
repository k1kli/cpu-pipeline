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

void SceneObject::SetMaterial(const Material& material)
{
	this->material = material;
}

Material& SceneObject::GetMaterial()
{
	return material;
}

const Material& SceneObject::GetMaterial() const
{
	return material;
}
