#pragma once
#include "RenderThread.h"
class RenderThreadManagement
{
public:
	void addToQueue(ScanLineProduct* scanLineProduct);
	void startThreads(const Scene& scene, FrameBuffer& fb,
		InterpolatorsManager& interpolatorsManager, const Interpolators& interpolators);
	void setRenderedObject(const SceneObject& renderedObject);
	~RenderThreadManagement();
private:
	RenderThread* renderThread = nullptr;
	std::thread* thread = nullptr;
	Buffer<ScanLineProduct> scanLineQueue;
};

