#pragma once
#include <vector>
#include "TriangleInterpolator.h"
#include "TriangleInterpolatorManagementInterface.h"
#include <glm/glm.hpp>
class InterpolatorsManager
{
	float a, b, c, d, e, f, g, h, i;
	std::vector<TriangleInterpolatorManagementInterface*> interpolators;
	int instanceCount;
public:
	InterpolatorsManager(int instanceCount) :instanceCount(instanceCount) {}
	void addInterpolator(TriangleInterpolatorManagementInterface& interpolator);
	void initTriangle(
		glm::vec4 v1, glm::vec4 v2, glm::vec4 v3);
	void updatePosition(int x, int y, int instance);
};

