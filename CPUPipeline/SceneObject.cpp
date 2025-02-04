#include "SceneObject.h"
#include <TransformationMatrices.h>

SceneObject::SceneObject(const Mesh& mesh, const Material& material, Transform transform)
	:mesh(mesh), material(material), transform(transform)
{
}

Mesh& SceneObject::GetMesh()
{
	return mesh;
}

const Mesh& SceneObject::GetMesh() const
{
	return mesh;
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

Transform& SceneObject::GetTransform()
{
	return transform;
}

const Transform& SceneObject::GetTransform() const
{
	return transform;
}

void SceneObject::load(SceneDataReader& reader)
{
	mesh.load(reader);
	material.load(reader);
	transform.load(reader);
}

void SceneObject::save(SceneDataWriter& writer) const
{
	mesh.save(writer);
	material.save(writer);
	transform.save(writer);
}
