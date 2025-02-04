#include "SceneRenderer.h"
#include "SceneObject.h"
#include "FrameBuffer.h"
#include <memory>
#include <algorithm>

SceneRenderer::SceneRenderer(FrameBuffer& frameBuffer):frameBuffer(frameBuffer)
{
	modelViewProjectionMatrix = glm::identity<glm::mat4>();
	viewProjectionMatrix = glm::identity<glm::mat4>();
	viewportMatrix = glm::identity<glm::mat4>();
	renderedObject = nullptr;
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
	renderThreadManagement.startThreads(*scene, frameBuffer, renderThreadCount);
	const Camera& camera = scene->getMainCamera();
	viewProjectionMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	viewportMatrix = camera.GetViewportMatrix();
	auto sceneObjects = scene->GetSceneObjects();
	for (auto i = 0; i < (int)sceneObjects.size(); i++)
	{
		renderedObject = sceneObjects[i];
		DrawSceneObject();
	}
	DrawLights();
	DrawCameras();
	glm::vec4 p = viewportMatrix * viewProjectionMatrix 
		* glm::vec4(camera.GetPosition() + camera.GetForward(),1);
	p /= p.w;
	frameBuffer.DrawRect((int)p.x - 10, (int)p.y - 10, (int)p.x + 10, (int)p.y + 10, RGBA(255, 255, 255, 50));
	renderThreadManagement.endThreads();
	for (int i = 0; i < (int)previousInterpolators.size(); i++)
	{
		delete previousInterpolators[i];
		delete previousInterpolatorsManagers[i];
		previousInterpolators[i] = nullptr;
		previousInterpolatorsManagers[i] = nullptr;
	}
	previousInterpolators.clear();
	previousInterpolatorsManagers.clear();
}
void SceneRenderer::toggleBackfaceCulling()
{
	backfaceCulling = !backfaceCulling;
}
void SceneRenderer::togglePerspectiveFix()
{
	perspectiveFix = !perspectiveFix;
}
void SceneRenderer::toggleWireframe()
{
	wireframe = !wireframe;
}
void SceneRenderer::selectObject(const SceneObject* objectToSelect)
{
	selectedObject = objectToSelect;
}
void SceneRenderer::selectLight(const Light* lightToSelect)
{
	selectedLight = lightToSelect;
}
void SceneRenderer::DrawSceneObject()
{
	TransformVertices();
	TransformNormals();
	DrawObjectsTriangles();
}
void SceneRenderer::TransformVertices()
{
	const glm::mat4 & worldMatrix = renderedObject->GetTransform().GetWorldMatrix();
	modelViewProjectionMatrix = viewProjectionMatrix* worldMatrix;
	auto vertices = renderedObject->GetMesh().getVertices();
	transformedVertices.resize(vertices.size());
	worldPosVertices.resize(vertices.size());
	for (auto i = 0; i < (int)vertices.size(); i++)
	{
		transformedVertices[i] = modelViewProjectionMatrix * glm::vec4(vertices[i], 1);
		worldPosVertices[i] = worldMatrix * glm::vec4(vertices[i], 1);
		worldPosVertices[i] /= worldPosVertices[i].w;
	}
}
void SceneRenderer::TransformNormals()
{
	glm::mat3 inverseWorldMatrix = glm::inverse(glm::transpose(renderedObject->GetTransform().GetWorldMatrix()));
	auto normals = renderedObject->GetMesh().getNormals();
	auto tbns = renderedObject->GetMesh().getTBN();
	transformedNormals.resize(normals.size());
	transformedTBN.resize(normals.size());
	for (auto i = 0; i < (int)normals.size(); i++)
	{
		transformedNormals[i] = inverseWorldMatrix * glm::vec4(normals[i], 0);
		transformedTBN[i] = inverseWorldMatrix * tbns[i];
	}
}
void SceneRenderer::DrawObjectsTriangles()
{
	const std::vector<glm::uvec3> triangles = renderedObject->GetMesh().getTriangles();
	for (auto i = 0; i < (int)triangles.size(); i++)
	{	
		glm::vec3 v1 = glm::vec3(transformedVertices[triangles[i].x]
			/ transformedVertices[triangles[i].x].w);
		glm::vec3 v2 = glm::vec3(transformedVertices[triangles[i].y]
			/ transformedVertices[triangles[i].y].w);
		glm::vec3 v3 = glm::vec3(transformedVertices[triangles[i].z]
			/ transformedVertices[triangles[i].z].w);
		glm::vec3 normal = glm::normalize(glm::cross(v2-v1, v3-v1));
		if (!backfaceCulling || dot({ 0,0,-1 }, normal) < 0)
		{
			DrawTriangle(i);
		}
	}
}
void SceneRenderer::DrawTriangle(int triangleId)
{

	DrawClippedTriangle(triangleId);
}
void SceneRenderer::DrawClippedTriangle(int triangleId)
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
		for (auto i = 0; i < (int)poly.size(); i++)
		{
			poly[i] = viewportMatrix * poly[i];
			poly[i] /= poly[i].w;
		}
		for (auto i = 1; i < (int)poly.size()-1; i++)
		{
			if(!wireframe)
				ScanLine(&poly[0], &poly[i], &poly[i + 1]);
			if (wireframe && renderedObject != selectedObject)
				WireFrame(&poly[0], &poly[i], &poly[i + 1], 0xFFFFFFFF);
			if (renderedObject == selectedObject)
				WireFrame(&poly[0], &poly[i], &poly[i + 1], 0xFF00FFFF);
		}
	}
}
void SceneRenderer::InitInterpolators(int triangleId,
	glm::vec4 v1InViewport,
	glm::vec4 v2InViewport,
	glm::vec4 v3InViewport)
{
	glm::uvec3 triangle = renderedObject->GetMesh().getTriangles()[triangleId];
	interpolatorsManager = new InterpolatorsManager(perspectiveFix);
	interpolators = new Interpolators(renderThreadCount);
	interpolatorsManager->addInterpolator(interpolators->worldPos);
	interpolatorsManager->addInterpolator(interpolators->uv);
	interpolatorsManager->addInterpolator(interpolators->tbn);
	interpolatorsManager->initTriangle(
		v1InViewport,
		v2InViewport,
		v3InViewport);
	glm::uvec3 triangleNormals = renderedObject->GetMesh().getTrianglesNormals()[triangleId];
	interpolators->tbn.initTriangleValues(
		transformedTBN[triangleNormals.x],
		transformedTBN[triangleNormals.y],
		transformedTBN[triangleNormals.z]);
	interpolators->worldPos.initTriangleValues(
		worldPosVertices[triangle.x],
		worldPosVertices[triangle.y],
		worldPosVertices[triangle.z]
	);
	glm::uvec3 triangleUV = renderedObject->GetMesh().getTrianglesUV()[triangleId];
	auto UVs = renderedObject->GetMesh().getUV();
	interpolators->uv.initTriangleValues(
		UVs[triangleUV.x],
		UVs[triangleUV.y],
		UVs[triangleUV.z]
	);
	previousInterpolators.push_back(interpolators);
	previousInterpolatorsManagers.push_back(interpolatorsManager);
}
void SceneRenderer::WireFrame(glm::vec4* v1, glm::vec4* v2, glm::vec4* v3, int color)
{
	frameBuffer.DrawLine(
		(int)v1->x,
		(int)v1->y,
		(int)v2->x,
		(int)v2->y, color);
	frameBuffer.DrawLine(
		(int)v2->x,
		(int)v2->y,
		(int)v3->x,
		(int)v3->y, color);
	frameBuffer.DrawLine(
		(int)v3->x,
		(int)v3->y,
		(int)v1->x,
		(int)v1->y, color);
}
void SceneRenderer::ScanLine(glm::vec4 * v1, glm::vec4 * v2, glm::vec4 * v3)
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
	if (glm::abs(v3->y - v1->y) < 1.0f) return;
	//point on the edge opposite to the middle point, which will be another vertex
	//of the base of two created triangles
	float v4q = (v2->y - v1->y) / (v3->y - v1->y);
	glm::vec4 v4 = *v1 * (1-v4q) + *v3 * v4q;
	if (v2->x < v4.x)
	{
		ScanLineHorizontalBase(*v2, v4, *v3);
		ScanLineHorizontalBase(*v2, v4, *v1);
	}
	else
	{
		ScanLineHorizontalBase(v4, *v2, *v3);
		ScanLineHorizontalBase(v4, *v2, *v1);
	}
}
void SceneRenderer::ScanLineHorizontalBase(
	const glm::vec3& v1baseLeft,
	const glm::vec3& v2baseRight,
	const glm::vec3& v3peak)
{
	int baseY = (int)v1baseLeft.y;
	int peakY = (int)v3peak.y;
	int yDiff = peakY - baseY;
	float depth1 = v1baseLeft.z;
	float depth2 = v2baseRight.z;
	float depth3 = v3peak.z;
	float minX = v1baseLeft.x;
	float maxX = v2baseRight.x;
	int yInc = glm::sign(peakY - baseY);
	float antitangent1 = (v3peak.x - v1baseLeft.x) / (v3peak.y - v1baseLeft.y) * yInc;
	float antitangent2 = (v3peak.x - v2baseRight.x) / (v3peak.y - v2baseRight.y) * yInc;
	for (int y = baseY; y != peakY+yInc; y += yInc)
	{
		float q = (float)(y - baseY) / yDiff;
		float lineDepth1 = depth1 * (1 - q) + depth3 * q;
		float lineDepth2 = depth2 * (1 - q) + depth3 * q;
		int xDiff = (int)(maxX - minX);
		if (xDiff == 0) return;
		renderThreadManagement.addToQueue(
			new ScanLineProduct(
				y, (int)minX, (int)maxX, lineDepth1, lineDepth2,
				interpolators, interpolatorsManager, renderedObject));
		minX += antitangent1;
		maxX += antitangent2;
	}
}

void SceneRenderer::DrawLights()
{
	for (Light* light : scene->GetLights())
	{
		glm::vec4 lightViewPos = viewProjectionMatrix
			* glm::vec4(light->getPosition(),1);
		if (triangleClipper.isPointVisible(lightViewPos))
		{
			lightViewPos = viewportMatrix * lightViewPos;
			lightViewPos /= lightViewPos.w;
			float lightSize = 80.0f /
				glm::length(scene->getMainCamera().GetPosition() - light->getPosition());
			int yMin = glm::max((int)(lightViewPos.y - lightSize / 2), 0);
			int yMax = glm::min((int)(lightViewPos.y + lightSize / 2), frameBuffer.getHeight() - 1);
			int xMin = glm::max((int)(lightViewPos.x - lightSize / 2), 0);
			int xMax = glm::min((int)(lightViewPos.x + lightSize / 2), frameBuffer.getWidth() - 1);
			int lightColor = floatToIntColor(glm::vec4(light->getDiffuseColor(), 1));

			for (int y = yMin; y <= yMax; y++)
			{
				for (int x = xMin; x <= xMax; x++)
				{
					frameBuffer.SetPixel(x, y, lightColor, lightViewPos.z);
				}
			}
			if (light == selectedLight)
			{
				int h = yMax - yMin;
				int w = xMax - xMin;
				yMin += h / 3;
				yMax -= h / 3;
				xMin += w / 3;
				xMax -= w / 3;
				for (int y = yMin; y <= yMax; y++)
				{
					for (int x = xMin; x <= xMax; x++)
					{
						frameBuffer.SetPixel(x, y, RGBA(0, 0, 255, 255), lightViewPos.z);
					}
				}
				h = yMax - yMin;
				w = xMax - xMin;
				yMin += h / 4;
				yMax -= h / 4;
				xMin += w / 4;
				xMax -= w / 4;
				for (int y = yMin; y <= yMax; y++)
				{
					for (int x = xMin; x <= xMax; x++)
					{
						frameBuffer.SetPixel(x, y, RGBA(255, 255, 0, 255), lightViewPos.z);
					}
				}
			}
		}
	}
}

void SceneRenderer::DrawCameras()
{
	for (Camera* camera : scene->getCameras())
	{
		if (camera == &scene->getMainCamera())
			continue;
		glm::vec4 cameraViewPos = viewProjectionMatrix
			* glm::vec4(camera->GetPosition(), 1);
		if (triangleClipper.isPointVisible(cameraViewPos))
		{
			cameraViewPos = viewportMatrix * cameraViewPos;
			cameraViewPos /= cameraViewPos.w;
			float lightSize = 80.0f /
				glm::length(scene->getMainCamera().GetPosition() - camera->GetPosition());
			int yMin = glm::max((int)(cameraViewPos.y - lightSize * 0.4f), 0);
			int yMax = glm::min((int)(cameraViewPos.y + lightSize * 0.4f), frameBuffer.getHeight() - 1);
			int xMin = glm::max((int)(cameraViewPos.x - lightSize * 0.33f), 0);
			int xMax = glm::min((int)(cameraViewPos.x + lightSize * 0.66f), frameBuffer.getWidth() - 1);

			for (int y = yMin; y <= yMax; y++)
			{
				for (int x = xMin; x <= xMax; x++)
				{
					frameBuffer.SetPixel(x, y, RGB(255,255,255), cameraViewPos.z);
				}
			}
			xMin = glm::max((int)(cameraViewPos.x - lightSize * 0.66f), 0);
			xMax = glm::min((int)(cameraViewPos.x - lightSize * 0.33f), frameBuffer.getWidth() - 1);
			int yDec = (int)((yMax - yMin) / (2.0f * (xMax - xMin)));
			for (int x = xMin; x <= xMax; x++, yMin +=yDec, yMax -= yDec)
			{
				for (int y = yMin; y <= yMax; y++)
				{
					frameBuffer.SetPixel(x, y, RGB(255, 255, 255), cameraViewPos.z);
				}
			}
		}
	}
}
