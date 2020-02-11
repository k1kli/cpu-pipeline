#include "StaticColorSampler.h"

glm::vec3 StaticColorSampler::sample(glm::vec2 uv) const
{
	return color;
}

const glm::vec3& StaticColorSampler::getColor()
{
	return color;
}
