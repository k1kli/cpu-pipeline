#include "InterpolatorsManager.h"

void InterpolatorsManager::addInterpolator(TriangleInterpolatorManagementInterface& interpolator)
{
	interpolators.push_back(&interpolator);
}

void InterpolatorsManager::initTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	detT = (v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y);
}


void InterpolatorsManager::updatePosition(int x, int y)
{
	float w1 = (v2.y - v3.y) * (x - v3.x) + (v3.x - v2.x) * (y - v3.y) / detT;
	float w2 = (v3.y - v1.y) * (x - v3.x) + (v1.x - v3.x) * (y - v3.y) / detT;
	float w3 = 1 - w2 - w1;
	w1 /= v1.z;
	w2 /= v2.z;
	w3 /= v3.z;
	for (auto i = 0; i < interpolators.size(); i++)
	{
		interpolators[i]->setBarycentricWeights(w1, w2, w3);
	}

}




