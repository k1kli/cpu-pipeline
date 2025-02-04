#include "RenderThread.h"


void RenderThread::operator()(void* params)
{
	while (true)
	{
		ScanLineProduct * scanLine = scanLineBuffer.remove();
		if (scanLine->interpolators == nullptr)
		{
			delete scanLine;
			break;
		}
		renderedObject = scanLine->renderedObject;
		interpolatorsManager = scanLine->interpolatorsManager;
		interpolators = scanLine->interpolators;
		int xDiff = scanLine->endX - scanLine->startX;
		for (int x = scanLine->startX; x <= scanLine->endX; x++)
		{
			interpolatorsManager->updatePosition(x, scanLine->y, id);
			float q = (float)(x - scanLine->startX) / xDiff;
			float depth = scanLine->startDepth * (1 - q) + scanLine->endDepth * q;
			fb.SetPixel(x, scanLine->y, GetPixelColor(), depth);
		}

		/*if(y % 20 == 0 && x % 20 == 0)
		{
			drawNormalLine(x, y);
		}
		else
		{
			frameBuffer.SetPixel(x, y, GetPixelColor(), depth);
		}*/
		delete scanLine;
	}
}

int RenderThread::GetPixelColor()
{
	const Material& material = renderedObject->GetMaterial();
	glm::vec3 ambientLight = { 1.0f, 1.0f, 1.0f };
	glm::vec2 uv = interpolators->uv.getValue(id);

	glm::mat3 tbn = interpolators->tbn.getValue(id);
	tbn[0] = glm::normalize(tbn[0]);
	tbn[1] = glm::normalize(tbn[1]);
	tbn[2] = glm::normalize(tbn[2]);

	glm::vec3 normal = glm::normalize(tbn * material.getNormalSampler().sample(
		uv));
	glm::vec3 worldPosition = interpolators->worldPos.getValue(id);

	glm::vec3 toObserver = glm::normalize(scene.getMainCamera().GetPosition() - worldPosition);

	glm::vec3 color = material.ambient * ambientLight;
	glm::vec3 objectColor = material.getColorSampler().sample(uv);
	for (Light* light : scene.GetLights())
	{
		glm::vec3 toLightVector = light->getPosition() - worldPosition;
		float dist = glm::length(toLightVector);
		toLightVector /= dist;
		glm::vec3 reflect =
			2 * glm::dot(toLightVector, normal) * normal - toLightVector;
		color +=
			(light->getDiffuseColor() *
				objectColor * glm::max(glm::dot(toLightVector, normal), 0.0f) +
				light->getSpecularColor() *
				material.specular * glm::pow(glm::max(glm::dot(reflect, toObserver), 0.0f), material.shininess))
			* light->getAttenuation(dist);
	}
	color = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
	return floatToIntColor(glm::vec4(color, 1));
}
