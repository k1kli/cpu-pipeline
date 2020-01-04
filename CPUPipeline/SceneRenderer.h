#pragma once
#include "Scene.h"
#include <FrameBuffer.h>
class SceneRenderer
{
	const Scene * scene = nullptr;
	FrameBuffer& frameBuffer;
	glm::mat4 vp;
	glm::mat4 mvp;
	void DrawSceneObject(const SceneObject& sceneObject, int color);
	void DrawTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, int color);
	void ScanLine(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int color);
	void ScanLineHorizontalBase(const glm::vec3& v1baseLeft, const glm::vec3& v2baseRight, const glm::vec3& v3peak, int color);
public:
	SceneRenderer(FrameBuffer& frameBuffer);
	void SetScene(const Scene & scene);
	void RenderScene();
};

