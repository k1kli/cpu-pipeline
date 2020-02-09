#include "RenderThreadManagement.h"

void RenderThreadManagement::addToQueue(ScanLineProduct* scanLineProduct)
{
	scanLineQueue.add(scanLineProduct);
}

void RenderThreadManagement::startThreads(const Scene& scene, FrameBuffer& fb,
	InterpolatorsManager& interpolatorsManager, const Interpolators& interpolators)
{
	renderThread = new RenderThread(scene, fb, interpolatorsManager, interpolators, scanLineQueue);
	thread = new std::thread(*renderThread, nullptr);
}

void RenderThreadManagement::setRenderedObject(const SceneObject& renderedObject)
{
	renderThread->setRenderedObject(renderedObject);
}

RenderThreadManagement::~RenderThreadManagement()
{
	if (renderThread != nullptr) delete renderThread;
	if (thread != nullptr)
	{
		scanLineQueue.add(new ScanLineProduct(INT32_MAX, 0, 0, 0, 0));
		thread->join();
		delete thread;
	}
}
