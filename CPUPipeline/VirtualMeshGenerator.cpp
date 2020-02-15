#include "VirtualMeshGenerator.h"
#include <string>
#include <memory>
#include "CuboidMeshGenerator.h"
#include "SphereMeshGenerator.h"
#include "CyllinderMeshGenerator.h"
#include "ConeMeshGenerator.h"
Mesh VirtualMeshGenerator::getMesh()
{
	resMesh = new Mesh(getInstance());
	validateParameters();
	resizeVectors();

	setValues();
	setTriangles();
	resMesh->setVertices(vertices);
	resMesh->setNormals(normals);
	resMesh->setTangents(tangents);
	resMesh->setTriangles(triangles);
	resMesh->setTrianglesNormals(trianglesNormals);
	resMesh->setTrianglesUV(trianglesUV);
	resMesh->setUV(uv);
	Mesh result = *resMesh;
	delete resMesh;
	return result;
}

const std::vector<std::string>& VirtualMeshGenerator::getParameterNames() const
{
	return parameterNames;
}

const std::vector<float>& VirtualMeshGenerator::getParameterValues() const
{
	return parameters;
}

const std::string& VirtualMeshGenerator::getName() const
{
	return meshName;
}

void VirtualMeshGenerator::setParameters(std::vector<float> parameters)
{
	this->parameters = parameters;
}

VirtualMeshGenerator* VirtualMeshGenerator::loadStatic(SceneDataReader& reader)
{
	MeshType type = (MeshType)reader.readInt();
	VirtualMeshGenerator * generator;
	switch (type)
	{
	case MeshType::CYLLINDER:
		generator = new CyllinderMeshGenerator();
		break;
	case MeshType::SPHERE:
		generator = new SphereMeshGenerator();
		break;
	case MeshType::CONE:
		generator = new ConeMeshGenerator();
		break;
	case MeshType::CUBE:
	default:
		generator = new CuboidMeshGenerator();
		break;
	}
	generator->load(reader);
	return generator;
}

void VirtualMeshGenerator::load(SceneDataReader& reader)
{
	int parametersSize = parameterNames.size();
	parameters.resize(parametersSize);
	for (int i = 0; i < parametersSize; i++)
	{
		parameters[i] = reader.readFloat();
	}
}

void VirtualMeshGenerator::save(SceneDataWriter& writer) const
{
	writer.write((int)getType());
	for (int i = 0; i < parameters.size(); i++)
	{
		writer.write(parameters[i]);
	}
}
