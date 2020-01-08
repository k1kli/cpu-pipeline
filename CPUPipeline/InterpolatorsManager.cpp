#include "InterpolatorsManager.h"

void InterpolatorsManager::addInterpolator(TriangleInterpolatorManagementInterface& interpolator)
{
	interpolators.push_back(&interpolator);
}

void InterpolatorsManager::initTriangle(int height, float vertBaseLeftDepth, float vertBaseRightDepth, float vertPeakDepth)
{
	this->triangleHeight = height;
	this->vertBaseLeftDepth = vertBaseLeftDepth;
	this->vertBaseRightDepth = vertBaseRightDepth;
	this->vertPeakDepth = vertPeakDepth;
	this->onTrianglePosition = 0;
}

void InterpolatorsManager::startLine(int lineWidth)
{
	float q = (float)onTrianglePosition / triangleHeight;
	leftDepth = vertBaseLeftDepth * (1 - q) + vertPeakDepth * q;
	rightDepth = vertBaseRightDepth * (1 - q) + vertPeakDepth * q;
	float qForInterpolators = q / vertPeakDepth;
	float oneMinusQForInterpolatorsLeft = (1 - q) / vertBaseLeftDepth;
	float oneMinusQForInterpolatorsRight = (1 - q) / vertBaseRightDepth;
	onLinePosition = 0;
	this->lineWidth = lineWidth;
	onTrianglePosition++;

	for (int i = 0; i < interpolators.size(); i++)
	{
		interpolators[i]->startLine(
			qForInterpolators,
			oneMinusQForInterpolatorsLeft,
			oneMinusQForInterpolatorsRight);
	}
}

void InterpolatorsManager::updatePosition()
{
	float q = (float)onLinePosition / lineWidth;
	float qForInterpolators = q * rightDepth;
	float oneMinusQForInterpolators = (1 - q) * leftDepth;
	for (int i = 0; i < interpolators.size(); i++)
	{
		interpolators[i]->setLinePosition(
			qForInterpolators,
			oneMinusQForInterpolators);
	}
	onLinePosition++;
}
