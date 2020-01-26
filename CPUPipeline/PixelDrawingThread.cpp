#include "PixelDrawingThread.h"
#include <memory>
#include <algorithm>

void PixelDrawingThread::setScene(const Scene& scene)
{
	this->scene = &scene;
}

void PixelDrawingThread::setRenderedObject(const SceneObject& renderedObject)
{
	this->renderedObject = &renderedObject;
}

void PixelDrawingThread::setPosition(int x, int y, float depth)
{
	this->x = x;
	this->y = y;
	this->depth = depth;
}

void PixelDrawingThread::operator()(void * params)
{
	while (workRemaining)
	{
		firstBarrier.Wait();
		//setRenderedObject, setScene, setPosition modifyValues only in main thread
		//they have to be brought here using pointers to them
		scene = *scenePtr;
		renderedObject = *renderedObjectPtr;
		secondBarrier.Wait();
		x = *xPtr;
		y = *yPtr;
		depth = *depthPtr;
		interpolatorsManager.updatePosition(x, y, id);
		frameBuffer.SetPixel(x, y, GetPixelColor(), depth);
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
int PixelDrawingThread::GetPixelColor()
{
	const Material& material = renderedObject->GetMaterial();
	glm::vec3 ambientLight = { 0.5f, 0.0f, 0.0f };
	glm::vec3 normal = glm::normalize(normalsInterpolator.getValue(id));
	glm::vec3 worldPosition = worldPosInterpolator.getValue(id);
	glm::vec3 toObserver = glm::normalize(scene->getMainCamera().GetPosition() - worldPosition);

	glm::vec3 color = material.ambient * ambientLight;

	for (Light* light : scene->GetLights())
	{
		glm::vec3 toLightVector = light->getPosition() - worldPosition;
		float dist = glm::length(toLightVector);
		toLightVector /= dist;
		glm::vec3 reflect = glm::normalize(
			2 * glm::dot(toLightVector, normal) * normal - toLightVector);
		color +=
			(light->getDiffuseColor() *
				material.diffuse * glm::dot(normal, toLightVector) +
				light->getSpecularColor() *
				material.specular * glm::pow(glm::dot(reflect, toObserver), material.shininess))
			* light->getAttenuation(dist);
	}
	color *= material.color;
	color = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
	return floatToIntColor(glm::vec4(color, 1));
}
