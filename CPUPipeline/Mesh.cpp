#include "Mesh.h"



Mesh::Mesh(std::shared_ptr<VirtualMeshGenerator> generator)
	:generator(generator)
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
	this->normals = normals;
}

void Mesh::setTangents(const std::vector<glm::vec3>& tangents)
{
	this->tbn.resize(tangents.size());
	for (int i = 0; i < tbn.size(); i++)
	{
		glm::vec3 normal = this->normals[i];
		glm::vec3 tangent = tangents[i];
		glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));
		this->tbn[i] = glm::mat3(tangent, binormal, normal);
		
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

void Mesh::setTrianglesUV(const std::vector<glm::uvec3>& trianglesUV)
{
	this->trianglesUV = trianglesUV;
}

void Mesh::setUV(const std::vector<glm::vec2>& uv)
{
	this->uv = uv;
}

const std::vector<glm::vec3>& Mesh::getVertices() const
{
	return vertices;
}

const std::vector<glm::vec3>& Mesh::getNormals() const
{
	return normals;
}


const std::vector<glm::mat3>& Mesh::getTBN() const
{
	return tbn;
}


const std::vector<glm::uvec3>& Mesh::getTriangles() const
{
	return triangles;
}

const std::vector<glm::uvec3>& Mesh::getTrianglesNormals() const
{
	return trianglesNormals;
}

const std::vector<glm::uvec3>& Mesh::getTrianglesUV() const
{
	return trianglesUV;
}

const std::vector<glm::vec2>& Mesh::getUV() const
{
	return uv;
}
