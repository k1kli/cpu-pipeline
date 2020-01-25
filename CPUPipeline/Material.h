#pragma once
#include <glm\glm.hpp>
class Material
{
public:
	float specular;
	float diffuse;
	float ambient;
	float shininess;
	glm::vec3 color;
	Material(float specular, float diffuse, float ambient, float shininess, glm::vec3 color)
		:specular(specular), diffuse(diffuse), ambient(ambient), shininess(shininess),
		color(color) {}
};

