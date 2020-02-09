#include "RenderThreadManagement.h"

void RenderThreadManagement::addToQueue(ScanLineProduct* scanLineProduct)
{
	scanLineQueue.add(scanLineProduct);
}

void RenderThreadManagement::startThreads(const Scene& scene, FrameBuffer& fb)
{
	renderThread = new RenderThread(scene, fb, scanLineQueue);
	thread = new std::thread(*renderThread, nullptr);
}

void RenderThreadManagement::endThreads()
{
	if (thread != nullptr)
	{
		scanLineQueue.add(new ScanLineProduct(0, 0, 0, 0, 0, nullptr, nullptr, nullptr));
		thread->join();
		delete thread;
	}
	if (renderThread != nullptr) delete renderThread;
	thread = nullptr;
	renderThread = nullptr;
}

RenderThreadManagement::~RenderThreadManagement()
{
	endThreads();
}
