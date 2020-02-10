#include "InterpolatorsManager.h"

void InterpolatorsManager::addInterpolator(TriangleInterpolatorManagementInterface& interpolator)
{
	interpolators.push_back(&interpolator);
}

void InterpolatorsManager::initTriangle(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
	this->v1 = v1/v1.w;
	v1DepthFix = v1.w;
	v2DepthFix = v2.w;
	v3DepthFix = v3.w;
	va = glm::vec2(v2 / v2DepthFix) - this->v1, vb = glm::vec2(v3/v3DepthFix) - this->v1;
	d00 = glm::dot(va, va);
	d01 = glm::dot(va, vb);
	d11 = glm::dot(vb, vb);
	denom = d00 * d11 - d01 * d01;
}


void InterpolatorsManager::updatePosition(int x, int y, int instance)
{
	glm::vec2 vc = glm::vec2({ x,y }) - v1;
	float d20 = glm::dot(vc, va);
	float d21 = glm::dot(vc, vb);
	float w2 = (d11 * d20 - d01 * d21) / denom;
	float w3 = (d00 * d21 - d01 * d20) / denom;
	float w1 = 1.0f - w3 - w2;
	w1 /= v1DepthFix;
	w2 /= v2DepthFix;
	w3 /= v3DepthFix;
	float sumInv = 1.0f/(w1 + w2 + w3);
	w1 *= sumInv;
	w2 *= sumInv;
	w3 *= sumInv;
	const int iSize = interpolators.size();
	for (auto i = 0; i < iSize; i++)
	{
		interpolators[i]->setBarycentricWeights(w1, w2, w3, instance);
	}

}




