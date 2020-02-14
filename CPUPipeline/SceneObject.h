#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
class SceneObject
{
	Mesh mesh;
	Material material;
	Transform transform;
public:
	SceneObject(const Mesh& mesh, const Material& material, Transform transform = Transform());
	Mesh& GetMesh();
	const Mesh& GetMesh() const;
	void SetMaterial(const Material & material);
	Material& GetMaterial();
	const Material& GetMaterial() const;
	Transform& GetTransform();
	const Transform& GetTransform() const;

};

