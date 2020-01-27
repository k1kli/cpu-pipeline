#include "StaticColorSampler.h"

glm::vec3 StaticColorSampler::sample(glm::vec2 uv) const
{
	return color;
}
