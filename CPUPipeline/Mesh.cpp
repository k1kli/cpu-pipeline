#include "Mesh.h"

Mesh::~Mesh()
{
}

void Mesh::setVertices(const std::vector<glm::vec3>& vertices)
{
	this->vertices.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		this->vertices[i] = vertices[i];
	}
}

void Mesh::setNormals(const std::vector<glm::vec3>& normals)
{
	this->normals.resize(normals.size());
	for (int i = 0; i < normals.size(); i++)
	{
		this->normals[i] = normals[i];
	}
}

void Mesh::setTriangles(const std::vector<glm::uvec3>& triangles)
{
	if (vertices.size() == 0)
	{
		throw "set vertices first";
	}
	for (int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].x >= vertices.size() ||
			triangles[i].y >= vertices.size() ||
			triangles[i].z >= vertices.size())
			throw "triangle num higher than vertices size";
	}
	this->triangles.resize(triangles.size());
	for (int i = 0; i < triangles.size(); i++)
	{
		this->triangles[i] = triangles[i];
	}
}

void Mesh::setTrianglesNormals(const std::vector<glm::uvec3>& trianglesNormals)
{
	if (normals.size() == 0)
	{
		throw "set normals first";
	}
	for (int i = 0; i < trianglesNormals.size(); i++)
	{
		if (trianglesNormals[i].x >= normals.size() ||
			trianglesNormals[i].y >= normals.size() ||
			trianglesNormals[i].z >= normals.size())
			throw "triangle num higher than vertices size";
	}
	this->trianglesNormals.resize(trianglesNormals.size());
	for (int i = 0; i < trianglesNormals.size(); i++)
	{
			this->trianglesNormals[i] = trianglesNormals[i];
	}
}

const std::vector<glm::vec3>& Mesh::getVertices() const
{
	return vertices;
}

const std::vector<glm::vec3>& Mesh::getNormals() const
{
	return normals;
}

const std::vector<glm::uvec3>& Mesh::getTriangles() const
{
	return triangles;
}

const std::vector<glm::uvec3>& Mesh::getTrianglesNormals() const
{
	return trianglesNormals;
}
