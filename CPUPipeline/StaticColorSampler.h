#pragma once
#include "Sampler.h"
class StaticColorSampler : public Sampler
{
	glm::vec3 color;
public:
	StaticColorSampler(const glm::vec3& color) :color(color) {}
	virtual glm::vec3 sample(glm::vec2 uv) const;
	virtual Sampler* getCopy() const;
	const glm::vec3& getColor() const;
};

