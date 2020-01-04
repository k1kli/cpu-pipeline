#include "SceneRenderer.h"
#include "SceneObject.h"


SceneRenderer::SceneRenderer(FrameBuffer& frameBuffer):frameBuffer(frameBuffer)
{
}

void SceneRenderer::SetScene(const Scene& scene)
{
	this->scene = &scene;
}

void SceneRenderer::RenderScene()
{
	if (scene == nullptr)
	{
		throw "Set scene first";
	}
	const Camera& camera = scene->getMainCamera();
	vp = camera.GetViewportMatrix() * camera.GetProjectionMatrix() * camera.GetViewMatrix();
	auto sceneObjects = scene->GetSceneObjects();
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		DrawSceneObject(*(sceneObjects[i]), 0xFFFF0000);
	}
}
void SceneRenderer::DrawSceneObject(const SceneObject & sceneObject, int color)
{
	mvp = vp * sceneObject.GetWorldMatrix();
	auto vertices = sceneObject.GetMesh().getVertices();
	std::vector<glm::vec3> modifiedVertices;
	modifiedVertices.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec4 vect = mvp * glm::vec4(vertices[i], 1);
		vect /= vect.w;
		modifiedVertices[i] = vect;
	}
	const std::vector<glm::uvec3> triangles = sceneObject.GetMesh().getTriangles();
	for (int i = 0; i < triangles.size(); i++)
	{
		glm::vec3 normal = glm::normalize(glm::cross(
			modifiedVertices[triangles[i].y] - modifiedVertices[triangles[i].x],
			modifiedVertices[triangles[i].z] - modifiedVertices[triangles[i].x]));
		if (dot({ 0,0,-1 }, normal) < 0)
		{
			DrawTriangle(
				modifiedVertices[triangles[i].x],
				modifiedVertices[triangles[i].y],
				modifiedVertices[triangles[i].z], color);
		}
	}
}
void SceneRenderer::DrawTriangle(
	const glm::vec3& v1,
	const glm::vec3& v2,
	const glm::vec3& v3, int color)
{
	ScanLine(v1, v2, v3, color);
	frameBuffer.DrawLine(
		v1.x,
		v1.y,
		v2.x,
		v2.y, 0xFF00FFFF);
	frameBuffer.DrawLine(
		v2.x,
		v2.y,
		v3.x,
		v3.y, 0xFF00FFFF);
	frameBuffer.DrawLine(
		v3.x,
		v3.y,
		v1.x,
		v1.y, 0xFF00FFFF);
}
void SceneRenderer::ScanLine(
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec3 v3, int color)
{
	glm::vec3 buf;


	if (v2.y < v1.y)
	{
		buf = v1;
		v1 = v2;
		v2 = buf;
	}
	if (v3.y < v2.y)
	{
		buf = v3;
		v3 = v2;
		v2 = buf;
		if (v2.y < v1.y)
		{
			buf = v1;
			v1 = v2;
			v2 = buf;
		}
	}
	//point on the edge opposite to the middle point, which will be another vertex
	//of the base of two created triangles
	glm::vec3 v4 = v1 + (v3 - v1) * ((v2.y - v1.y) / (v3.y - v1.y));
	if (v2.x < v4.x)
	{
		ScanLineHorizontalBase(v2, v4, v3, color);
		ScanLineHorizontalBase(v2, v4, v1, color);
	}
	else
	{
		ScanLineHorizontalBase(v4, v2, v3, color);
		ScanLineHorizontalBase(v4, v2, v1, color);
	}
}
void SceneRenderer::ScanLineHorizontalBase(
	const glm::vec3& v1baseLeft,
	const glm::vec3& v2baseRight,
	const glm::vec3& v3peak, int color)
{
	int baseY = v1baseLeft.y;
	int peakY = v3peak.y;
	int yDiff = peakY - baseY;
	float depth1 = v1baseLeft.z;
	float depth2 = v2baseRight.z;
	float depth3 = v3peak.z;
	float minX = v1baseLeft.x;
	float maxX = v2baseRight.x;
	int yInc = glm::sign(peakY - baseY);
	float antitangent1 = (v3peak.x - v1baseLeft.x) / (v3peak.y - v1baseLeft.y) * yInc;
	float antitangent2 = (v3peak.x - v2baseRight.x) / (v3peak.y - v2baseRight.y) * yInc;
	for (int y = baseY; y != peakY; y += yInc)
	{
		float q = (float)(y - baseY) / yDiff;
		float lineDepth1 = depth1 * (1 - q) + depth3 * q;
		float lineDepth2 = depth2 * (1 - q) + depth3 * q;
		int xDiff = maxX - minX;
		for (int x = minX; x < maxX; x++)
		{
			q = (float)(x - (int)minX) / xDiff;
			float depth = lineDepth1 * (1 - q) + lineDepth2 * q;
			frameBuffer.SetPixel(x, y, color, depth);
		}
		minX += antitangent1;
		maxX += antitangent2;
	}

}