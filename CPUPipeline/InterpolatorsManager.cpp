#include "InterpolatorsManager.h"

void InterpolatorsManager::addInterpolator(TriangleInterpolatorManagementInterface& interpolator)
{
	interpolators.push_back(&interpolator);
}

void InterpolatorsManager::initTriangle(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	float detT = (v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y);
	a = (v2.y - v3.y) / detT;
	b = (v3.x - v2.x) / detT;
	c = (v3.y - v1.y) / detT;
	d = (v1.x - v3.x) / detT;
	e = -v3.x * a - v3.y * b;
	f = -v3.x * c - v3.y * d;
	g = -a - c;
	h = -b - d;
	i = 1 - e - f;
	a *= v1.w; b *= v1.w; e *= v1.w;
	c *= v2.w; d *= v2.w; f *= v2.w;
	g *= v3.w; h *= v3.w; i *= v3.w;
}


void InterpolatorsManager::updatePosition(int x, int y)
{
	float w1 = a * x + b * y + e;
	float w2 = c * x + d * y + f;
	float w3 = g * x + h * y + i;
	float sumInv = 1.0f/(w1 + w2 + w3);
	w1 *= sumInv;
	w2 *= sumInv;
	w3 *= sumInv;
	const int iSize = interpolators.size();
	for (auto i = 0; i < iSize; i++)
	{
		interpolators[i]->setBarycentricWeights(w1, w2, w3);
	}

}




