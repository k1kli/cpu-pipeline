#pragma once
#include "TriangleInterpolator.h"
#include "glm/glm.hpp"
struct Interpolators
{
	Interpolators(int instaceCount)
		:normal(instaceCount), tbn(instaceCount), worldPos(instaceCount), uv(instaceCount) {};
	TriangleInterpolator<glm::vec3> normal;
	TriangleInterpolator<glm::mat3> tbn;
	TriangleInterpolator<glm::vec3> worldPos;
	TriangleInterpolator<glm::vec2> uv;
};