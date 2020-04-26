#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "SaveableObject.h"
class SceneObject : public SaveableObject
{
	Mesh mesh;
	Material material;
	Transform transform;
public:
	SceneObject(const Mesh& mesh, const Material& material, Transform transform = Transform());
	SceneObject(SceneDataReader& reader) { load(reader); }
	Mesh& GetMesh();
	const Mesh& GetMesh() const;
	void SetMaterial(const Material & material);
	Material& GetMaterial();
	const Material& GetMaterial() const;
	Transform& GetTransform();
	const Transform& GetTransform() const;
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;

};

