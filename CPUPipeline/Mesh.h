#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
class Mesh
{
	std::vector<glm::uvec3> triangles;
	std::vector<glm::uvec3> trianglesNormals;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
public:
	virtual ~Mesh();
	void setVertices(const std::vector<glm::vec3> & vertices);
	void setNormals(const std::vector<glm::vec3> & normals);
	void setTriangles(const std::vector<glm::uvec3>& triangles);
	void setTrianglesNormals(const std::vector<glm::uvec3>& trianglesNormals);
	const std::vector<glm::vec3>& getVertices() const;
	const std::vector<glm::vec3>& getNormals() const;
	const std::vector<glm::uvec3>& getTriangles() const;
	const std::vector<glm::uvec3>& getTrianglesNormals() const;
};

