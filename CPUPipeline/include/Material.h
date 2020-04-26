#pragma once
#include <glm\glm.hpp>
#include "Sampler.h"
#include <memory>
#include "SceneDataReader.h"
#include "SceneDataWriter.h"
class Material : public SaveableObject
{
public:
	glm::vec3 specular = { 0,0,0 };
	glm::vec3 ambient = { 0,0,0 };
	float shininess = 0;
	Material(
		glm::vec3 specular,
		glm::vec3 ambient, float shininess,
		const Sampler & colorSampler,
		const Sampler& normalSampler)
		:specular(specular),
		ambient(ambient), shininess(shininess),
		colorSampler(colorSampler.getCopy()),
		normalSampler(normalSampler.getCopy()) {}
	Material() {};
	~Material();
	Material(const Material& other);
	Material& operator=(const Material& other);

	const Sampler& getColorSampler() const;
	const Sampler& getNormalSampler() const;
	void setColorSampler(const Sampler& newSampler);
	void setNormalSampler(const Sampler& newSampler);
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;
private:
	Sampler* colorSampler = nullptr;
	Sampler* normalSampler = nullptr;

};

