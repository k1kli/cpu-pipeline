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
	for (auto i = 0; i < sceneObjects.size(); i++)
	{
		renderedObject = sceneObjects[i];
		DrawSceneObject(0xFFFF0000);
	}
	DrawLights();
	renderThreadManagement.endThreads();
	for (int i = 0; i < previousInterpolators.size(); i++)
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
void SceneRenderer::DrawSceneObject(int color)
{
	TransformVertices();
	TransformNormals();
	DrawObjectsTriangles(color);
}
void SceneRenderer::TransformVertices()
{
	const glm::mat4 & worldMatrix = renderedObject->GetTransform().GetWorldMatrix();
	modelViewProjectionMatrix = viewProjectionMatrix* worldMatrix;
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
	glm::mat3 inverseWorldMatrix = glm::inverse(glm::transpose(renderedObject->GetTransform().GetWorldMatrix()));
	auto normals = renderedObject->GetMesh().getNormals();
	auto tbns = renderedObject->GetMesh().getTBN();
	transformedNormals.resize(normals.size());
	transformedTBN.resize(normals.size());
	for (auto i = 0; i < normals.size(); i++)
	{
		transformedNormals[i] = inverseWorldMatrix * glm::vec4(normals[i], 0);
		transformedTBN[i] = inverseWorldMatrix * tbns[i];
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
		if (!backfaceCulling || dot({ 0,0,-1 }, normal) < 0)
		{
			DrawTriangle(i, color);
		}
	}
}
void SceneRenderer::DrawTriangle(int triangleId, int color)
{

	DrawClippedTriangle(triangleId, color);
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
			if(wireframe)
				WireFrame(&poly[0], &poly[i], &poly[i + 1], 0xFF00FFFF);
			else
			{
				ScanLine(&poly[0], &poly[i], &poly[i + 1], color);
			}
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
		v1->x,
		v1->y,
		v2->x,
		v2->y, 0xFF00FFFF);
	frameBuffer.DrawLine(
		v2->x,
		v2->y,
		v3->x,
		v3->y, 0xFF00FFFF);
	frameBuffer.DrawLine(
		v3->x,
		v3->y,
		v1->x,
		v1->y, 0xFF00FFFF);
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
	if (glm::abs(v3->y - v1->y) < 1.0f) return;
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
	for (int y = baseY; y != peakY; y += yInc)
	{
		float q = (float)(y - baseY) / yDiff;
		float lineDepth1 = depth1 * (1 - q) + depth3 * q;
		float lineDepth2 = depth2 * (1 - q) + depth3 * q;
		int xDiff = maxX - minX;
		if (xDiff == 0) return;
		renderThreadManagement.addToQueue(
			new ScanLineProduct(
				y, minX, maxX, lineDepth1, lineDepth2,
				interpolators, interpolatorsManager, renderedObject));
		minX += antitangent1;
		maxX += antitangent2;
	}
}
//int floatToIntColor(const glm::vec4& floatColor)
//{
//	uint8_t r = (uint8_t)(std::clamp<float>(floatColor.r, 0.f, 1.f) * 255);
//	uint8_t g = (uint8_t)(std::clamp<float>(floatColor.g, 0.f, 1.f) * 255);
//	uint8_t b = (uint8_t)(std::clamp<float>(floatColor.b, 0.f, 1.f) * 255);
//	uint8_t a = (uint8_t)(std::clamp<float>(floatColor.a, 0.f, 1.f) * 255);
//	return RGBA(r, g, b, a);
//}
//int SceneRenderer::GetPixelColor()
//{
//	const Material& material = renderedObject->GetMaterial();
//	glm::vec3 ambientLight = { 1.0f, 1.0f, 1.0f };
//	glm::vec2 uv = interpolators.uv.getValue();
//
//	glm::vec3 baseNormal = glm::normalize(interpolators.normal.getValue());
//	glm::mat3 tbn = interpolators.tbn.getValue();
//	tbn[0] = glm::normalize(tbn[0]);
//	tbn[1] = glm::normalize(tbn[1]);
//	tbn[2] = baseNormal;
//	
//	glm::vec3 normal = glm::normalize(tbn * material.normalSampler->sample(
//		uv));
//	glm::vec3 worldPosition = interpolators.worldPos.getValue();
//
//	glm::vec3 toObserver = glm::normalize(scene->getMainCamera().GetPosition() - worldPosition);
//
//	glm::vec3 color = material.ambient * ambientLight;
//	glm::vec3 objectColor = material.colorSampler->sample(uv);
//	for (Light* light : scene->GetLights())
//	{
//		glm::vec3 toLightVector = light->getPosition()-worldPosition;
//		float dist = glm::length(toLightVector);
//		toLightVector /= dist;
//		glm::vec3 reflect =
//			2 * glm::dot(toLightVector, normal) * normal - toLightVector;
//		color +=
//			(light->getDiffuseColor() *
//				objectColor * glm::max(glm::dot(toLightVector, normal), 0.0f) +
//				light->getSpecularColor() *
//				material.specular * glm::pow(glm::max(glm::dot(reflect, toObserver),0.0f), material.shininess))
//			* light->getAttenuation(dist);
//	}
//	color = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
//	return floatToIntColor(glm::vec4(color, 1));
//}

//void SceneRenderer::drawNormalLine(int x, int y)
//{
//	glm::mat3 tbn = interpolators->tbn.getValue(0);
//	glm::vec2 uv = interpolators->uv.getValue(0);
//	const Material& material = renderedObject->GetMaterial();
//	glm::vec3 normal = glm::normalize(tbn * material.normalSampler->sample(uv));
//	glm::vec4 lineEndWorldPos = glm::vec4(interpolators->worldPos.getValue(0)
//		+ normal * 0.02f, 1);
//	glm::vec4 lineEndViewPos = viewportMatrix
//		* viewProjectionMatrix * lineEndWorldPos;
//	lineEndViewPos /= lineEndViewPos.w;
//	frameBuffer.DrawLine(x, y, lineEndViewPos.x, lineEndViewPos.y, 0xFFFF00FF);
//}

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
		}
	}
}