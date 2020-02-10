#pragma once
#include "Scene.h"
#include <FrameBuffer.h>
#include "InterpolatorsManager.h"
#include "TriangleInterpolator.h"
#include "TriangleClipper.h"
#include "Interpolators.h"
#include "RenderThreadManagement.h"
class SceneRenderer
{
	const int renderThreadCount = 8;
	const Scene * scene = nullptr;
	FrameBuffer& frameBuffer;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 viewportMatrix;
	Interpolators * interpolators;
	InterpolatorsManager* interpolatorsManager;
	std::vector<Interpolators*> previousInterpolators;
	std::vector<InterpolatorsManager*> previousInterpolatorsManagers;
	std::vector<glm::vec4> transformedVertices;
	std::vector<glm::vec3> transformedNormals;
	std::vector<glm::mat3> transformedTBN;
	std::vector<glm::vec4> worldPosVertices;
	TriangleClipper triangleClipper;
	RenderThreadManagement renderThreadManagement;
	bool backfaceCulling = true;
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
	void WireFrame(glm::vec4* v1, glm::vec4* v2, glm::vec4* v3, int color);
	//int GetPixelColor();
	//void drawNormalLine(int x, int y);
	void DrawLights();
	void ScanLine(glm::vec4* v1, glm::vec4* v2, glm::vec4* v3, int color);
	void ScanLineHorizontalBase(const glm::vec3& v1baseLeft, const glm::vec3& v2baseRight, const glm::vec3& v3peak, int color);
public:
	SceneRenderer(FrameBuffer& frameBuffer);
	void SetScene(const Scene & scene);
	void RenderScene();
	void toggleBackfaceCulling();
};

