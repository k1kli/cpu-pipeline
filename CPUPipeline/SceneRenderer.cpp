#include "SceneRenderer.h"
#include "SceneObject.h"
#include "FrameBuffer.h"
#include <memory>
#include <algorithm>


SceneRenderer::SceneRenderer(FrameBuffer& frameBuffer)
	:frameBuffer(frameBuffer) ,interpolatorsManager(512)
{
	modelViewProjectionMatrix = glm::identity<glm::mat4>();
	viewProjectionMatrix = glm::identity<glm::mat4>();
	viewportMatrix = glm::identity<glm::mat4>();
	renderedObject = nullptr;
	interpolatorsManager.addInterpolator(normalInterpolator);
	interpolatorsManager.addInterpolator(worldPosInterpolator);
	
	for (int i = 0; i < 512; i++)
	{
		PixelDrawingThreadManagement * pdtm
			= new PixelDrawingThreadManagement(
				frameBuffer, i, interpolatorsManager, normalInterpolator,
			worldPosInterpolator);
		pixelDrawingThreadPool.push_back(pdtm);
	}
}

SceneRenderer::~SceneRenderer()
{
	for (int i = 0; i < pixelDrawingThreadPool.size(); i++)
	{
		delete pixelDrawingThreadPool[i];
	}
}

void SceneRenderer::SetScene(const Scene& scene)
{
	this->scene = &scene;
	for (auto pixelThread : pixelDrawingThreadPool)
	{
		pixelThread->thread->setScene(scene);
	}
}

void SceneRenderer::RenderScene()
{
	if (scene == nullptr)
	{
		throw "Set scene first";
	}
	const Camera& camera = scene->getMainCamera();
	viewProjectionMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	viewportMatrix = camera.GetViewportMatrix();
	auto sceneObjects = scene->GetSceneObjects();
	for (auto i = 0; i < sceneObjects.size(); i++)
	{
		renderedObject = sceneObjects[i];
		DrawSceneObject(0xFFFF0000);
	}
}
void SceneRenderer::DrawSceneObject(int color)
{
	for (auto pixelThread : pixelDrawingThreadPool)
	{
		pixelThread->thread->setRenderedObject(*renderedObject);
	}
	TransformVertices();
	TransformNormals();
	DrawObjectsTriangles(color);
}
void SceneRenderer::TransformVertices()
{
	const glm::mat4 & worldMatrix = renderedObject->GetWorldMatrix();
	modelViewProjectionMatrix = viewProjectionMatrix*renderedObject->GetWorldMatrix();
	auto vertices = renderedObject->GetMesh().getVertices();
	transformedVertices.resize(vertices.size());
	worldPosVertices.resize(vertices.size());
	for (auto i = 0; i < vertices.size(); i++)
	{
		transformedVertices[i] = modelViewProjectionMatrix * glm::vec4(vertices[i], 1);
		worldPosVertices[i] = worldMatrix * glm::vec4(vertices[i], 1);
		worldPosVertices[i] /= worldPosVertices[i].w;
	}
}
void SceneRenderer::TransformNormals()
{
	glm::mat4 inverseWorldMatrix = glm::inverse(glm::transpose(renderedObject->GetWorldMatrix()));
	auto normals = renderedObject->GetMesh().getNormals();
	transformedNormals.resize(normals.size());
	for (auto i = 0; i < normals.size(); i++)
	{
		glm::vec4 vect = inverseWorldMatrix * glm::vec4(normals[i], 0);
		transformedNormals[i] = vect;
	}
}
void SceneRenderer::DrawObjectsTriangles(int color)
{
	const std::vector<glm::uvec3> triangles = renderedObject->GetMesh().getTriangles();
	for (auto i = 0; i < triangles.size(); i++)
	{	
		glm::vec3 v1 = glm::vec3(transformedVertices[triangles[i].x]
			/ transformedVertices[triangles[i].x].w);
		glm::vec3 v2 = glm::vec3(transformedVertices[triangles[i].y]
			/ transformedVertices[triangles[i].y].w);
		glm::vec3 v3 = glm::vec3(transformedVertices[triangles[i].z]
			/ transformedVertices[triangles[i].z].w);
		glm::vec3 normal = glm::normalize(glm::cross(v2-v1, v3-v1));
		if (dot({ 0,0,-1 }, normal) < 0)
		{
			DrawTriangle(i, color);
		}
	}
}
void SceneRenderer::DrawTriangle(int triangleId, int color)
{

	DrawClippedTriangle(triangleId, color);
	//WireFrame(triangleId, 0xFF00FFFF);
}
void SceneRenderer::DrawClippedTriangle(int triangleId, int color)
{
	glm::uvec3 mainTriangle = renderedObject->GetMesh().getTriangles()[triangleId];
	
	glm::vec4 v1 = transformedVertices[mainTriangle.x];
	glm::vec4 v2 = transformedVertices[mainTriangle.y];
	glm::vec4 v3 = transformedVertices[mainTriangle.z];
	std::vector<glm::vec4> poly = { v1,v2,v3 };
	triangleClipper.ClipTriangle(poly);
	if (poly.size() >= 3)
	{
		v1 = viewportMatrix * v1;
		v2 = viewportMatrix * v2;
		v3 = viewportMatrix * v3;
		InitInterpolators(triangleId, v1, v2, v3);
		for (auto i = 0; i < poly.size(); i++)
		{
			poly[i] = viewportMatrix * poly[i];
			poly[i] /= poly[i].w;
		}
		for (auto i = 1; i < poly.size()-1; i++)
		{
			ScanLine(&poly[0], &poly[i], &poly[i + 1], color);
		}
	}
}
void SceneRenderer::InitInterpolators(int triangleId,
	glm::vec4 v1InViewport,
	glm::vec4 v2InViewport,
	glm::vec4 v3InViewport)
{
	glm::uvec3 triangle = renderedObject->GetMesh().getTriangles()[triangleId];
	interpolatorsManager.initTriangle(
		v1InViewport,
		v2InViewport,
		v3InViewport);
	glm::uvec3 triangleNormals = renderedObject->GetMesh().getTrianglesNormals()[triangleId];
	normalInterpolator.initTriangleValues(
		transformedNormals[triangleNormals.x],
		transformedNormals[triangleNormals.y],
		transformedNormals[triangleNormals.z]);
	worldPosInterpolator.initTriangleValues(
		worldPosVertices[triangle.x],
		worldPosVertices[triangle.y],
		worldPosVertices[triangle.z]
	);
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
void SceneRenderer::ScanLine(glm::vec4 * v1, glm::vec4 * v2, glm::vec4 * v3, int color)
{
	glm::vec4 * buf;

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
	glm::vec4 v4 = *v1 * (1-v4q) + *v3 * v4q;
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
	int threadId = 0;
	for (int y = baseY; y != peakY; y += yInc)
	{
		float q = (float)(y - baseY) / yDiff;
		float lineDepth1 = depth1 * (1 - q) + depth3 * q;
		float lineDepth2 = depth2 * (1 - q) + depth3 * q;
		int xDiff = maxX - minX;
		for (int x = minX; x <= maxX; x++)
		{
			q = (float)(x - (int)minX) / xDiff;
			float depth = lineDepth1 * (1 - q) + lineDepth2 * q;
			pixelDrawingThreadPool[threadId]->firstBarrier.Wait();
			pixelDrawingThreadPool[threadId]->thread->setPosition(x, y, depth);
			pixelDrawingThreadPool[threadId]->secondBarrier.Wait();
			threadId = (threadId + 1) % pixelDrawingThreadPool.size();
		}
		minX += antitangent1;
		maxX += antitangent2;
	}
}
