#pragma once
#include <vector>
#include "TriangleInterpolator.h"
#include "TriangleInterpolatorManagementInterface.h"
class InterpolatorsManager
{
	float vertBaseLeftDepth;
	float vertBaseRightDepth;
	float vertPeakDepth;
	int triangleHeight;
	int lineWidth;
	int onTrianglePosition;
	int onLinePosition;
	float leftDepth;
	float rightDepth;
	std::vector<TriangleInterpolatorManagementInterface*> interpolators;
public:
	void addInterpolator(TriangleInterpolatorManagementInterface& interpolator);
	void initTriangle(
		int height,
		float vertBaseLeftDepth,
		float vertBaseRightDepth,
		float vertPeakDepth);
	void startLine(int lineWidth);
	void updatePosition();
};

