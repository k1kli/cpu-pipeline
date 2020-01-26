#include "PixelDrawingThreadManagement.h"

PixelDrawingThreadManagement::PixelDrawingThreadManagement(
	FrameBuffer& frameBuffer, int id,
	InterpolatorsManager& interpolatorsManager,
	TriangleInterpolator<glm::vec3>& normalsInterpolator,
	TriangleInterpolator<glm::vec3>& worldPosInterpolator)
{
	thread = new PixelDrawingThread(frameBuffer, firstBarrier, secondBarrier,
		hasWorkRemaining, id, interpolatorsManager,
		normalsInterpolator, worldPosInterpolator);
	t = new std::thread(*thread, nullptr);
}

PixelDrawingThreadManagement::~PixelDrawingThreadManagement()
{
	hasWorkRemaining = false;
	t->join();
	delete thread;
	delete t;
}
