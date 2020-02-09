#pragma once
class TriangleInterpolatorManagementInterface
{
public:
	virtual void setBarycentricWeights(float w1, float w2, float w3, int instance) = 0;
};

