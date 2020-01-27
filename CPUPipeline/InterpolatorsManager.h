#pragma once
#include <vector>
#include "TriangleInterpolator.h"
#include "TriangleInterpolatorManagementInterface.h"
#include <glm/glm.hpp>
class InterpolatorsManager
{
	//float a, b, c, d, e, f, g, h, i;
	glm::vec2 v1;
	float v1DepthFix, v2DepthFix, v3DepthFix;
	float denom;
	float d00;
	float d01;
	float d11;
	glm::vec2 va, vb;
	std::vector<TriangleInterpolatorManagementInterface*> interpolators;
public:
	void addInterpolator(TriangleInterpolatorManagementInterface& interpolator);
	void initTriangle(
		glm::vec4 v1, glm::vec4 v2, glm::vec4 v3);
	void updatePosition(int x, int y);
};

