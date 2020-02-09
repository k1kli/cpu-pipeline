#pragma once
#include "TriangleInterpolator.h"
#include "glm/glm.hpp"
struct Interpolators
{
	TriangleInterpolator<glm::vec3> normal;
	TriangleInterpolator<glm::mat3> tbn;
	TriangleInterpolator<glm::vec3> worldPos;
	TriangleInterpolator<glm::vec2> uv;
};