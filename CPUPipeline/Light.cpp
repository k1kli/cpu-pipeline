#include "Light.h"



const glm::vec3& Light::getPosition() const
{
	return position;
}

const glm::vec3& Light::getDiffuseColor() const
{
	return diffuseColor;
}

const glm::vec3& Light::getSpecularColor() const
{
	return specularColor;
}

float Light::getAttenuation(const float& distanceFromLight) const
{
	return 1.0f/(attenuationConstant
		+attenuationLinear*distanceFromLight
		+attenuationQuadratic*distanceFromLight*distanceFromLight);
}

float Light::getAttenuationConstant() const
{
	return attenuationConstant;
}

float Light::getAttenuationLinear() const
{
	return attenuationLinear;
}

float Light::getAttenuationQuadratic() const
{
	return attenuationQuadratic;
}

void Light::setPosition(const glm::vec3& newPos)
{
	position = newPos;
}

void Light::setDiffuseColor(const glm::vec3& newDiffuse)
{
	diffuseColor = newDiffuse;
}

void Light::setSpecularColor(const glm::vec3& newSpecular)
{
	specularColor = newSpecular;
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	attenuationConstant = constant;
	attenuationLinear = linear;
	attenuationQuadratic = quadratic;
}

void Light::load(SceneDataReader& reader)
{
	position = reader.readVec3();
	diffuseColor = reader.readVec3();
	specularColor = reader.readVec3();
	attenuationConstant = reader.readFloat();
	attenuationLinear = reader.readFloat();
	attenuationQuadratic = reader.readFloat();
}

void Light::save(SceneDataWriter& writer) const
{
	writer.write(position);
	writer.write(diffuseColor);
	writer.write(specularColor);
	writer.write(attenuationConstant);
	writer.write(attenuationLinear);
	writer.write(attenuationQuadratic);
}
