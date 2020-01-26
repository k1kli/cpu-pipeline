#pragma once
#include "PixelDrawingThread.h"
#include "Barrier.h"
class PixelDrawingThreadManagement
{
	std::thread * t;
public:
	PixelDrawingThreadManagement(
		FrameBuffer& frameBuffer, int id,
		InterpolatorsManager& interpolatorsManager,
		TriangleInterpolator<glm::vec3>& normalsInterpolator,
		TriangleInterpolator<glm::vec3>& worldPosInterpolator);
	~PixelDrawingThreadManagement();
	PixelDrawingThread* thread;
	Barrier firstBarrier = Barrier(2);
	Barrier secondBarrier = Barrier(2);
	bool hasWorkRemaining = true;
};

