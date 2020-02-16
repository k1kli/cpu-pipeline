#include "RenderThreadManagement.h"

void RenderThreadManagement::addToQueue(ScanLineProduct* scanLineProduct)
{
	scanLineQueue.add(scanLineProduct);
}

void RenderThreadManagement::startThreads(const Scene& scene, FrameBuffer& fb, int threadCount)
{
	renderThreads.resize(threadCount);
	threads.resize(threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		renderThreads[i] = new RenderThread(scene, fb, scanLineQueue, i);
		threads[i] = new std::thread(*(renderThreads[i]), nullptr);
	}
}

void RenderThreadManagement::endThreads()
{
	for (int i = 0; i < (int)threads.size(); i++)
	{
		scanLineQueue.add(new ScanLineProduct(0, 0, 0, 0, 0, nullptr, nullptr, nullptr));
	}
	for (int i = 0; i < (int)threads.size(); i++)
	{
		threads[i]->join();
		delete threads[i];
		delete renderThreads[i];
	}
	threads.clear();
	renderThreads.clear();
}

RenderThreadManagement::~RenderThreadManagement()
{
	endThreads();
}
