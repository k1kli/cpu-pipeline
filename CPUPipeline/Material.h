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
	Material(
		glm::vec3 specular,
		glm::vec3 ambient, float shininess,
		const Sampler & colorSampler,
		const Sampler& normalSampler)
		:specular(specular),
		ambient(ambient), shininess(shininess),
		colorSampler(colorSampler.getCopy()),
		normalSampler(normalSampler.getCopy()) {}
	~Material();
	Material(const Material& other);
	Material& operator=(const Material& other);

	const Sampler& getColorSampler() const;
	const Sampler& getNormalSampler() const;
	void setColorSampler(const Sampler& newSampler);
	void setNormalSampler(const Sampler& newSampler);
private:
	Sampler* colorSampler;
	Sampler* normalSampler;

};

