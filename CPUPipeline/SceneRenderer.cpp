#include "SceneRenderer.h"
#include "SceneObject.h"
#include "FrameBuffer.h"
#include <memory>
#include <algorithm>


SceneRenderer::SceneRenderer(FrameBuffer& frameBuffer):frameBuffer(frameBuffer)
{
	mvp = glm::identity<glm::mat4>();
	vp = glm::identity<glm::mat4>();
	interpolatorsManager.addInterpolator(normalInterpolator);
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
		renderedObject = sceneObjects[i];
		DrawSceneObject(0xFFFF0000);
	}
}
void SceneRenderer::DrawSceneObject(int color)
{
	TransformVertices();
	TransformNormals();
	DrawObjectsTriangles(color);
}
void SceneRenderer::TransformVertices()
{
	mvp = vp * renderedObject->GetWorldMatrix();
	auto vertices = renderedObject->GetMesh().getVertices();
	transformedVertices.resize(vertices.size());
	verticesDepthsForInterpolation.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec4 vect = mvp * glm::vec4(vertices[i], 1);
		verticesDepthsForInterpolation[i] = vect.w;
		vect /= vect.w;
		transformedVertices[i] = vect;
	}
}
void SceneRenderer::TransformNormals()
{
	glm::mat4 inverseWorldMatrix = glm::inverse(renderedObject->GetWorldMatrix());
	auto normals = renderedObject->GetMesh().getNormals();
	transformedNormals.resize(normals.size());
	for (int i = 0; i < normals.size(); i++)
	{
		glm::vec4 vect = inverseWorldMatrix * glm::vec4(normals[i], 0);
		transformedNormals[i] = vect;
	}
}
void SceneRenderer::DrawObjectsTriangles(int color)
{
	const std::vector<glm::uvec3> triangles = renderedObject->GetMesh().getTriangles();
	for (int i = 0; i < triangles.size(); i++)
	{
		glm::vec3 normal = glm::normalize(glm::cross(
			transformedVertices[triangles[i].y] - transformedVertices[triangles[i].x],
			transformedVertices[triangles[i].z] - transformedVertices[triangles[i].x]));
		if (dot({ 0,0,-1 }, normal) < 0)
		{
			DrawTriangle(i, color);
		}
	}
}
void SceneRenderer::DrawTriangle(int triangleId, int color)
{

	InitInterpolators(triangleId);
	DrawClippedTriangle(triangleId, color);
	WireFrame(triangleId, 0xFF00FFFF);
}
void SceneRenderer::DrawClippedTriangle(int triangleId, int color)
{
	glm::uvec3 mainTriangle = renderedObject->GetMesh().getTriangles()[triangleId];
	ScanLine(
		&transformedVertices[mainTriangle.x],
		&transformedVertices[mainTriangle.y],
		&transformedVertices[mainTriangle.z], color);
}
void SceneRenderer::InitInterpolators(int triangleId)
{
	glm::uvec3 triangle = renderedObject->GetMesh().getTriangles()[triangleId];
	interpolatorsManager.initTriangle(
		transformedVertices[triangle.x],
		transformedVertices[triangle.y],
		transformedVertices[triangle.z]);
	glm::uvec3 triangleNormals = renderedObject->GetMesh().getTrianglesNormals()[triangleId];
	normalInterpolator.initTriangleValues(
		transformedNormals[triangleNormals.x],
		transformedNormals[triangleNormals.y],
		transformedNormals[triangleNormals.z]);
}
void SceneRenderer::WireFrame(int triangleId, int color)
{
	const std::vector<glm::uvec3> triangles = renderedObject->GetMesh().getTriangles();
	const glm::vec3& v1 = transformedVertices[triangles[triangleId].x];
	const glm::vec3& v2 = transformedVertices[triangles[triangleId].y];
	const glm::vec3& v3 = transformedVertices[triangles[triangleId].z];
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
void SceneRenderer::ScanLine(glm::vec3 * v1, glm::vec3 * v2, glm::vec3 * v3, int color)
{
	glm::vec3 * buf;

	if (v2->y < v1->y)
	{
		buf = v1;
		v1 = v2;
		v2 = buf;
	}
	if (v3->y < v2->y)
	{
		buf = v3;
		v3 = v2;
		v2 = buf;
		if (v2->y < v1->y)
		{
			buf = v1;
			v1 = v2;
			v2 = buf;
		}
	}
	//point on the edge opposite to the middle point, which will be another vertex
	//of the base of two created triangles
	float v4q = (v2->y - v1->y) / (v3->y - v1->y);
	glm::vec3 v4 = *v1 * (1-v4q) + *v3 * v4q;
	if (v2->x < v4.x)
	{
		ScanLineHorizontalBase(*v2, v4, *v3, color);
		ScanLineHorizontalBase(*v2, v4, *v1, color);
	}
	else
	{
		ScanLineHorizontalBase(v4, *v2, *v3, color);
		ScanLineHorizontalBase(v4, *v2, *v1, color);
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
			interpolatorsManager.updatePosition(x, y);
			q = (float)(x - (int)minX) / xDiff;
			float depth = lineDepth1 * (1 - q) + lineDepth2 * q;
			frameBuffer.SetPixel(x, y, GetPixelColor(), depth);
		}
		minX += antitangent1;
		maxX += antitangent2;
	}
}
int floatToIntColor(const glm::vec4& floatColor)
{
	uint8_t r = (uint8_t)(std::clamp<float>(floatColor.r, 0.f, 1.f) * 255);
	uint8_t g = (uint8_t)(std::clamp<float>(floatColor.g, 0.f, 1.f) * 255);
	uint8_t b = (uint8_t)(std::clamp<float>(floatColor.b, 0.f, 1.f) * 255);
	uint8_t a = (uint8_t)(std::clamp<float>(floatColor.a, 0.f, 1.f) * 255);
	return RGBA(r, g, b, a);
}
int SceneRenderer::GetPixelColor()
{
	glm::vec3 baseColor = { 1,1,1 };
	glm::vec3 normal = glm::normalize(normalInterpolator.getValue());
	baseColor *= normal;
	return floatToIntColor(glm::vec4(baseColor, 1));
}