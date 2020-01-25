#pragma once
#include "Mesh.h"
#include "Material.h"
class SceneObject
{
	Mesh mesh;
	glm::mat4 worldMatrix;
	Material material;
public:
	SceneObject(const Mesh& mesh, const glm::mat4& worldMatrix, const Material& material)
		:mesh(mesh), worldMatrix(worldMatrix), material(material) {}
	Mesh& GetMesh();
	const Mesh& GetMesh() const;
	const glm::mat4& GetWorldMatrix() const;
	void SetWorldMatrix(const glm::mat4& worldMatrix);
	void SetMaterial(const Material & material);
	Material& GetMaterial();
	const Material& GetMaterial() const;

};

