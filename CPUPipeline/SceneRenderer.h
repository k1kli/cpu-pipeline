#pragma once
#include "Scene.h"
#include <FrameBuffer.h>
#include "InterpolatorsManager.h"
#include "TriangleInterpolator.h"
#include "TriangleClipper.h"
class SceneRenderer
{
	const Scene * scene = nullptr;
	FrameBuffer& frameBuffer;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 viewportMatrix;
	std::vector<glm::vec4> transformedVertices;
	std::vector<glm::vec3> transformedNormals;
	std::vector<glm::vec4> worldPosVertices;
	InterpolatorsManager interpolatorsManager;
	TriangleInterpolator<glm::vec3> normalInterpolator;
	TriangleInterpolator<glm::vec3> worldPosInterpolator;
	TriangleInterpolator<glm::vec2> uvInterpolator;
	TriangleClipper triangleClipper;
	const SceneObject * renderedObject;
	void DrawSceneObject(int color);
	void TransformVertices();
	void TransformNormals();
	void DrawObjectsTriangles(int color);
	void DrawTriangle(int triangleId, int color);
	void DrawClippedTriangle(int triangleId, int color);
	void InitInterpolators(int triangleId,
		glm::vec4 v1InViewport,
		glm::vec4 v2InViewport,
		glm::vec4 v3InViewport);
	int GetPixelColor();
	void WireFrame(int triangleId, int color);
	void ScanLine(glm::vec4* v1, glm::vec4* v2, glm::vec4* v3, int color);
	void ScanLineHorizontalBase(const glm::vec3& v1baseLeft, const glm::vec3& v2baseRight, const glm::vec3& v3peak, int color);
public:
	SceneRenderer(FrameBuffer& frameBuffer);
	void SetScene(const Scene & scene);
	void RenderScene();
};

