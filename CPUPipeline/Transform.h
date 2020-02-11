#pragma once
#include <TransformationMatrices.h>
class Transform
{
public:
	Transform();

	glm::mat4 GetWorldMatrix() const;
	const glm::vec3& getScale() const;
	const glm::vec3& getEulerAngles() const;
	const glm::vec3& getPosition() const;
	void SetScale(const glm::vec3& newScale);
	void SetEulerAngles(const glm::vec3& newEulerAngles);
	void SetPosition(const glm::vec3& newPosition);
private:
	glm::mat4 translation;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 scaling;
	glm::vec3 translationVector;
	glm::vec3 eulerAnglesVector;
	glm::vec3 scalingVector;
};

