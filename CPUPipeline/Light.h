#pragma once
#include <glm/glm.hpp>
#include "SaveableObject.h"
class Light : public SaveableObject
{
	glm::vec3 position;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
public:
	Light(SceneDataReader& reader) { load(reader); }
	Light(const glm::vec3& position,
		const glm::vec3& diffuseColor,
		const glm::vec3& specularColor,
		const float& attenuationConstant,
		const float& attenuationLinear,
		const float& attenuationQuadratic)
		:position(position), diffuseColor(diffuseColor),
		specularColor(specularColor), attenuationConstant(attenuationConstant),
		attenuationLinear(attenuationLinear), attenuationQuadratic(attenuationQuadratic) {}
	const glm::vec3& getPosition() const;
	const glm::vec3& getDiffuseColor() const;
	const glm::vec3& getSpecularColor() const;
	float getAttenuation(const float& distanceFromLight) const;
	float getAttenuationConstant() const;
	float getAttenuationLinear() const;
	float getAttenuationQuadratic() const;
	void setPosition(const glm::vec3& newPos);
	void setDiffuseColor(const glm::vec3& newDiffuse);
	void setSpecularColor(const glm::vec3& newSpecular);
	void setAttenuation(float constant, float linear, float quadratic);
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;

};

