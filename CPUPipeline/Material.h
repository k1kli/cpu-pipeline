#pragma once
#include <glm\glm.hpp>
#include "Sampler.h"
#include <memory>
class Material
{
public:
	float specular;
	float diffuse;
	float ambient;
	float shininess;
	std::shared_ptr<Sampler> colorSampler;
	std::shared_ptr<Sampler> normalSampler;
	Material(
		float specular, float diffuse,
		float ambient, float shininess,
		std::shared_ptr<Sampler> colorSampler,
		std::shared_ptr<Sampler> normalSampler)
		:specular(specular), diffuse(diffuse),
		ambient(ambient), shininess(shininess),
		colorSampler(colorSampler),
		normalSampler(normalSampler) {}

};

