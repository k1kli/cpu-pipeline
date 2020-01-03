#include "TransformationMatrices.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
namespace TransformationMatrices {
	glm::mat4 getRotationMatrix(float angle, glm::vec3 axis)
	{
		float s = sin(angle);
		float c = cos(angle);
		glm::mat4 rotation;
		rotation[0][0] = axis.x*axis.x+(1-axis.x*axis.x)*c;
		rotation[0][1] = axis.x*axis.y*(1-c)+axis.z*s;
		rotation[0][2] = axis.x*axis.z*(1-c)-axis.y*s;
		rotation[0][3] = 0;

		rotation[1][0] = axis.x * axis.y * (1 - c) + axis.z * s;
		rotation[1][1] = axis.y * axis.y + (1 - axis.y * axis.y) * c;
		rotation[1][2] = axis.y * axis.z * (1 - c) + axis.x * s;
		rotation[1][3] = 0;

		rotation[2][0] = axis.x * axis.z * (1 - c) + axis.y * s;
		rotation[2][1] = axis.y * axis.z * (1 - c) - axis.x * s;
		rotation[2][2] = axis.z * axis.z + (1 - axis.z * axis.z) * c;
		rotation[2][3] = 0;

		rotation[3][0] = 0;
		rotation[3][1] = 0;
		rotation[3][2] = 0;
		rotation[3][3] = 1;
		return rotation;
	}
	glm::mat4 getTranslationMatrix(glm::vec3 translateVector)
	{
		glm::mat4 translation;
		translation[0][0] = 1;
		translation[0][1] = 0;
		translation[0][2] = 0;
		translation[0][3] = 0;

		translation[1][0] = 0;
		translation[1][1] = 1;
		translation[1][2] = 0;
		translation[1][3] = 0;

		translation[2][0] = 0;
		translation[2][1] = 0;
		translation[2][2] = 1;
		translation[2][3] = 0;

		translation[3][0] = translateVector.x;
		translation[3][1] = translateVector.y;
		translation[3][2] = translateVector.z;
		translation[3][3] = 1;
		return translation;
	}
	glm::mat4 getScalingMatrix(glm::vec3 scaleVector)
	{
		glm::mat4 scaling;
		scaling[0][0] = scaleVector.x;
		scaling[0][1] = 0;
		scaling[0][2] = 0;
		scaling[0][3] = 0;

		scaling[1][0] = 0;
		scaling[1][1] = scaleVector.y;
		scaling[1][2] = 0;
		scaling[1][3] = 0;

		scaling[2][0] = 0;
		scaling[2][1] = 0;
		scaling[2][2] = scaleVector.z;
		scaling[2][3] = 0;

		scaling[3][0] = 0;
		scaling[3][1] = 0;
		scaling[3][2] = 0;
		scaling[3][3] = 1;
		return scaling;
	}
}