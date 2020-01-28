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
	const Sampler * normalSampler;
	Material(
		float specular, float diffuse,
		float ambient, float shininess,
		const Sampler& colorSampler,
		const Sampler & normalSampler)
		:specular(specular), diffuse(diffuse),
		ambient(ambient), shininess(shininess),
		colorSampler(&colorSampler),
		normalSampler(&normalSampler) {}
};

