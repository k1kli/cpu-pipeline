#pragma once
#include "InterpolatorsManager.h"
#include "Scene.h"
#include "Interpolators.h"
#include "FrameBuffer.h"
#include "Buffer.h"
#include "ScanLineProduct.h"
class RenderThread
{
public:
	RenderThread(const Scene& scene, FrameBuffer& fb,
		InterpolatorsManager& interpolatorsManager, const Interpolators& interpolators,
		Buffer<ScanLineProduct> & scanLineBuffer)
		:scene(scene), fb(fb), interpolatorsManager(interpolatorsManager), interpolators(interpolators),
		scanLineBuffer(scanLineBuffer){}
	void setRenderedObject(const SceneObject& renderedObject);
	void operator()(void* params);

	int GetPixelColor();

private:
	const Scene& scene;
	FrameBuffer& fb;
	Buffer<ScanLineProduct>& scanLineBuffer;
	InterpolatorsManager& interpolatorsManager;
	const Interpolators& interpolators;
	const SceneObject* renderedObject = nullptr;
	const SceneObject** renderedObjectPtr = &renderedObject;

};

