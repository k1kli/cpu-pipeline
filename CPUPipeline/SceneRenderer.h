#pragma once
#include "Scene.h"
#include <FrameBuffer.h>
#include "InterpolatorsManager.h"
#include "TriangleInterpolator.h"
class SceneRenderer
{
	const Scene * scene = nullptr;
	FrameBuffer& frameBuffer;
	glm::mat4 vp;
	glm::mat4 mvp;
	std::vector<float> verticesDepthsForInterpolation;
	std::vector<glm::vec3> transformedVertices;
	std::vector<glm::vec3> transformedNormals;
	InterpolatorsManager interpolatorsManager;
	TriangleInterpolator<glm::vec3> normalInterpolator;
	const SceneObject * renderedObject;
	void DrawSceneObject(int color);
	void TransformVertices();
	void TransformNormals();
	void DrawObjectsTriangles(int color);
	void DrawTriangle(int triangleId, int color);
	int GetPixelColor();
	void WireFrame(int triangleId, int color);
	void ScanLine(int triangleId, int color);
	void ScanLineHorizontalBase(const glm::vec3& v1baseLeft, const glm::vec3& v2baseRight, const glm::vec3& v3peak, int color);
public:
	SceneRenderer(FrameBuffer& frameBuffer);
	void SetScene(const Scene & scene);
	void RenderScene();
};

