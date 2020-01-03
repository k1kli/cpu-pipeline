#pragma once
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
namespace TransformationMatrices {
	glm::mat4 getRotationMatrix(float angle, glm::vec3 axis);
	glm::mat4 getTranslationMatrix(glm::vec3 translateVector);
	glm::mat4 getScalingMatrix(glm::vec3 scaleVector);
}

