#pragma once
class TriangleInterpolatorManagementInterface
{
public:
	virtual void startLine(float q, float oneMinusQLeft, float oneMinusQRight) = 0;
	virtual void setLinePosition(float q, float oneMinusQ) = 0;
};

