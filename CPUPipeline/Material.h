#pragma once
#include <glm\glm.hpp>
#include "Sampler.h"
class Material
{
public:
	float specular;
	float diffuse;
	float ambient;
	float shininess;
	const Sampler * colorSampler;
	Material(
		float specular, float diffuse,
		float ambient, float shininess,
		const Sampler& colorSampler)
		:specular(specular), diffuse(diffuse),
		ambient(ambient), shininess(shininess),
		colorSampler(&colorSampler) {}
};

