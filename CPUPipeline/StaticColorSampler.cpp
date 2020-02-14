#include "StaticColorSampler.h"

glm::vec3 StaticColorSampler::sample(glm::vec2 uv) const
{
	return color;
}

Sampler* StaticColorSampler::getCopy() const
{
	return new StaticColorSampler(*this);
}

const glm::vec3& StaticColorSampler::getColor() const
{
	return color;
}
