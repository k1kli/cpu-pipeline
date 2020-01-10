#pragma once
#include <vector>
#include "TriangleInterpolator.h"
#include "TriangleInterpolatorManagementInterface.h"
#include <glm/glm.hpp>
class InterpolatorsManager
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	float detT;
	std::vector<TriangleInterpolatorManagementInterface*> interpolators;
public:
	void addInterpolator(TriangleInterpolatorManagementInterface& interpolator);
	void initTriangle(
		glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	void updatePosition(int x, int y);
};

