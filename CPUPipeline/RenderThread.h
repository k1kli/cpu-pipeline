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
		Buffer<ScanLineProduct> & scanLineBuffer)
		:scene(scene), fb(fb),
		scanLineBuffer(scanLineBuffer){}
	void operator()(void* params);

	int GetPixelColor();

private:
	const Scene& scene;
	FrameBuffer& fb;
	Buffer<ScanLineProduct>& scanLineBuffer;
	InterpolatorsManager * interpolatorsManager = nullptr;
	const Interpolators * interpolators = nullptr;
	const SceneObject* renderedObject = nullptr;

};

