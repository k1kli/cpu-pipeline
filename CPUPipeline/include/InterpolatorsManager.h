#pragma once
#include <vector>
#include "TriangleInterpolator.h"
#include "TriangleInterpolatorManagementInterface.h"
#include <glm/glm.hpp>
class InterpolatorsManager
{
	//float a, b, c, d, e, f, g, h, i;
	glm::vec2 v1 = { -1.0f,-1.0f };
	float v1DepthFix = -1.0f, v2DepthFix = -1.0f, v3DepthFix = -1.0f;
	float denom = -1.0f;
	float d00 = -1.0f;
	float d01 = -1.0f;
	float d11 = -1.0f;
	glm::vec2 va = { -1.0f,-1.0f }, vb = { -1.0f,-1.0f };
	std::vector<TriangleInterpolatorManagementInterface*> interpolators;
	bool perspectiveFix;
public:
	InterpolatorsManager(bool perspectiveFix) :perspectiveFix(perspectiveFix) {}
	void addInterpolator(TriangleInterpolatorManagementInterface& interpolator);
	void initTriangle(
		glm::vec4 v1, glm::vec4 v2, glm::vec4 v3);
	void updatePosition(int x, int y, int instance);
};

