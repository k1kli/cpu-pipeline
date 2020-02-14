#pragma once
#include <glm/glm.hpp>
class Sampler 
{
public:
	virtual glm::vec3 sample(glm::vec2 uv) const = 0;
	virtual Sampler* getCopy() const = 0;
};