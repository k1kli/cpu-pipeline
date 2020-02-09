#pragma once
#include "RenderThread.h"
class RenderThreadManagement
{
public:
	void addToQueue(ScanLineProduct* scanLineProduct);
	void startThreads(const Scene& scene, FrameBuffer& fb);
	void endThreads();
	~RenderThreadManagement();
private:
	RenderThread* renderThread = nullptr;
	std::thread* thread = nullptr;
	Buffer<ScanLineProduct> scanLineQueue;
};

