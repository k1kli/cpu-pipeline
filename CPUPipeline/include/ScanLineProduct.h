#pragma once
#include "Interpolators.h"
#include "InterpolatorsManager.h"
#include "SceneObject.h"
struct ScanLineProduct
{
	ScanLineProduct(int y, int startX, int endX, float startDepth, float endDepth,
		const Interpolators * interpolators, InterpolatorsManager * interpolatorsManager,
		const SceneObject * renderedObject)
		:y(y), startX(startX), endX(endX), startDepth(startDepth), endDepth(endDepth),
		interpolators(interpolators), interpolatorsManager(interpolatorsManager),
		renderedObject(renderedObject){}
	int y;
	int startX;
	int endX;
	float startDepth;
	float endDepth;
	const Interpolators * interpolators;
	InterpolatorsManager * interpolatorsManager;
	const SceneObject* renderedObject;
};

