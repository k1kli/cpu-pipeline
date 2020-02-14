#include "Transform.h"

Transform::Transform()
{
	translation = glm::identity<glm::mat4>();
	rotationX = glm::identity<glm::mat4>();
	rotationY = glm::identity<glm::mat4>();
	rotationZ = glm::identity<glm::mat4>();
	scaling = glm::identity<glm::mat4>();

	translationVector = { 0,0,0 };
	eulerAnglesVector = { 0,0,0 };
	scalingVector = { 1,1,1 };
}

glm::mat4 Transform::GetWorldMatrix() const
{
	return translation * rotationY * rotationX * rotationZ * scaling;
}

const glm::vec3& Transform::getScale() const
{
	return scalingVector;
}

const glm::vec3& Transform::getEulerAngles() const
{
	return eulerAnglesVector;
}

const glm::vec3& Transform::getPosition() const
{
	return translationVector;
}

void Transform::SetScale(const glm::vec3& newScale)
{
	if (newScale != scalingVector)
	{
		scalingVector = newScale;
		scaling = TransformationMatrices::getScalingMatrix(scalingVector);
	}
}

void Transform::SetEulerAngles(const glm::vec3& newEulerAngles)
{
	if (newEulerAngles != eulerAnglesVector)
	{
		eulerAnglesVector = newEulerAngles;
		rotationX = TransformationMatrices::getXRotationMatrix(newEulerAngles.x);
		rotationY = TransformationMatrices::getYRotationMatrix(newEulerAngles.y);
		rotationZ = TransformationMatrices::getZRotationMatrix(newEulerAngles.z);
	}
}
void Transform::SetPosition(const glm::vec3& newPosition)
{
	if (newPosition != translationVector)
	{
		translationVector = newPosition;
		translation = TransformationMatrices::getTranslationMatrix(translationVector);
	}
}
