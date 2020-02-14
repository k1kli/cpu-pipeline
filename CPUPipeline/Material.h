#pragma once
#include <glm\glm.hpp>
#include "Sampler.h"
#include <memory>
class Material
{
public:
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
	std::shared_ptr<Sampler> colorSampler;
	std::shared_ptr<Sampler> normalSampler;
	Material(
		glm::vec3 specular,
		glm::vec3 ambient, float shininess,
		std::shared_ptr<Sampler> colorSampler,
		std::shared_ptr<Sampler> normalSampler)
		:specular(specular),
		ambient(ambient), shininess(shininess),
		colorSampler(colorSampler),
		normalSampler(normalSampler) {}

};

