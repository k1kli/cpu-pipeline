#include "VirtualMeshGenerator.h"
#include <string>
#include <memory>

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
