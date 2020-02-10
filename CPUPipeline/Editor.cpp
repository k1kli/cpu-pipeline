#include "Editor.h"
#include <TransformationMatrices.h>

void Editor::processInput(float deltaTime)
{
	currentWidth = fb.getWidth();
	currentHeight = fb.getHeight();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	moveCamera(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		sceneRenderer.toggleBackfaceCulling();
}
void Editor::moveCamera(float deltaTime)
{
	Camera& camera = scene->getMainCamera();
	const glm::vec3& forward = camera.GetForward();
	const glm::vec3& up = camera.GetUp();
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 right = glm::cross(forward, up);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += forward * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= right * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= forward * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += right * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		pos += up * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		pos -= up * deltaTime;
	camera.LookAt(pos, forward, up);
}

void Editor::processMouse(double xPos, double yPos)
{
	currentWidth = fb.getWidth();
	currentHeight = fb.getHeight();
	xPos = xPos - currentWidth / 2.0;
	yPos = yPos - currentHeight / 2.0;
	double xDiff = xPos - oldXPos;
	double yDiff = yPos - oldYPos;
	oldXPos = xPos;
	oldYPos = yPos;
	rotateCamera(xDiff, yDiff);
	
}
void Editor::rotateCamera(double xDiff, double yDiff)
{
	yDiff /= currentHeight / 2.0;
	Camera& camera = scene->getMainCamera();
	glm::vec3 forward = camera.GetForward();
	const glm::vec3& up = camera.GetUp();
	if (abs(yDiff) > 0.001)
	{
		glm::vec3 right = glm::cross(forward, up);
		forward = TransformationMatrices::getRotationMatrix(-(float)yDiff * 0.6f, right)
			* glm::vec4(forward, 1);

	}
	xDiff /= currentWidth / 2.0;
	if (abs(xDiff) > 0.001)
	{
		forward = TransformationMatrices::getRotationMatrix(-(float)xDiff * 1.6f, up)
			* glm::vec4(forward, 1);
	}
	camera.LookAt(camera.GetPosition(), forward, up);
}