#pragma once
#include "Mesh.h"
class VirtualMeshGenerator
{
public:
	Mesh getMesh();
private:

	Mesh resMesh;
protected:

	virtual void validateParameters() = 0;
	virtual void resizeVectors() = 0;
	virtual void setValues() = 0;
	virtual void setTriangles() = 0;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::uvec3> triangles;
	std::vector<glm::uvec3> trianglesNormals;
	std::vector<glm::vec2> uv;
	std::vector<glm::uvec3> trianglesUV;
};

