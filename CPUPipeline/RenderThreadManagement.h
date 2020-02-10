#pragma once
#include "RenderThread.h"
class RenderThreadManagement
{
public:
	void addToQueue(ScanLineProduct* scanLineProduct);
	void startThreads(const Scene& scene, FrameBuffer& fb, int threadCount);
	void endThreads();
	~RenderThreadManagement();
private:
	std::vector<RenderThread*> renderThreads;
	std::vector <std::thread*> threads;
	Buffer<ScanLineProduct> scanLineQueue;
};

