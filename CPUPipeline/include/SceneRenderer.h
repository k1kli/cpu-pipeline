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
	Interpolators * interpolators = nullptr;
	InterpolatorsManager* interpolatorsManager = nullptr;
	std::vector<Interpolators*> previousInterpolators;
	std::vector<InterpolatorsManager*> previousInterpolatorsManagers;
	std::vector<glm::vec4> transformedVertices;
	std::vector<glm::vec3> transformedNormals;
	std::vector<glm::mat3> transformedTBN;
	std::vector<glm::vec4> worldPosVertices;
	TriangleClipper triangleClipper;
	RenderThreadManagement renderThreadManagement;
	bool backfaceCulling = true;
	bool perspectiveFix = true;
	bool wireframe = false;
	const SceneObject * renderedObject;
	const SceneObject* selectedObject = nullptr;
	const Light* selectedLight = nullptr;
	void DrawSceneObject();
	void TransformVertices();
	void TransformNormals();
	void DrawObjectsTriangles();
	void DrawTriangle(int triangleId);
	void DrawClippedTriangle(int triangleId);
	void InitInterpolators(int triangleId,
		glm::vec4 v1InViewport,
		glm::vec4 v2InViewport,
		glm::vec4 v3InViewport);
	void WireFrame(glm::vec4* v1, glm::vec4* v2, glm::vec4* v3, int color);
	void DrawLights();
	void DrawCameras();
	void ScanLine(glm::vec4* v1, glm::vec4* v2, glm::vec4* v3);
	void ScanLineHorizontalBase(
		const glm::vec3& v1baseLeft,
		const glm::vec3& v2baseRight,
		const glm::vec3& v3peak);
public:
	SceneRenderer(FrameBuffer& frameBuffer);
	void SetScene(const Scene & scene);
	void RenderScene();
	void toggleBackfaceCulling();
	void togglePerspectiveFix();
	void toggleWireframe();
	void selectObject(const SceneObject* objectToSelect);
	void selectLight(const Light* lightToSelect);
};

