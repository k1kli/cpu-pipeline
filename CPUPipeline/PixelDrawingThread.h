#pragma once
#include "Barrier.h"
#include <FrameBuffer.h>
#include "Scene.h"
#include "InterpolatorsManager.h"
class PixelDrawingThread
{
	FrameBuffer & frameBuffer;
	Barrier & firstBarrier;
	Barrier & secondBarrier;
	const bool & workRemaining;
	const int id;
	const Scene * scene = nullptr;
	const SceneObject* renderedObject = nullptr;
	const Scene** scenePtr = &scene;
	const SceneObject ** renderedObjectPtr = &renderedObject;
	InterpolatorsManager & interpolatorsManager;
	TriangleInterpolator<glm::vec3> & normalsInterpolator;
	TriangleInterpolator<glm::vec3> & worldPosInterpolator;
	int x=0;
	int y=0;
	float depth = 0.0f;
	int* xPtr = &x, * yPtr = &y;
	float *depthPtr = &depth;
	int GetPixelColor();
public:
	PixelDrawingThread(
		FrameBuffer& frameBuffer, Barrier& firstBarrier,
		Barrier& secondBarrier, const bool& workRemaining, int id,
		InterpolatorsManager & interpolatorsManager,
		TriangleInterpolator<glm::vec3> & normalsInterpolator,
		TriangleInterpolator<glm::vec3> & worldPosInterpolator)
		:frameBuffer(frameBuffer), firstBarrier(firstBarrier),
		secondBarrier(secondBarrier), workRemaining(workRemaining),
		id(id), interpolatorsManager(interpolatorsManager),
		normalsInterpolator(normalsInterpolator), worldPosInterpolator(worldPosInterpolator)
	{}
	void setScene(const Scene & scene);
	void setRenderedObject(const SceneObject& renderedObject);
	void setPosition(int x, int y, float depth);
	void operator()(void * params);
};

