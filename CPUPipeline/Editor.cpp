#include "Editor.h"
#include <TransformationMatrices.h>
#include "Raycast.h"
#include <algorithm>
#include "CreateObjectScreen.h"

void Editor::handleInput(float deltaTime)
{
	if(input.getKey(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	if (currentScreen != nullptr)
	{
		currentScreen->handleInput(input);
		return;
	}
	moveCamera(deltaTime);
	rotateCamera();
	if(input.getKeyDown(GLFW_KEY_P))
		sceneRenderer.toggleBackfaceCulling();
	if (input.getKeyDown(GLFW_KEY_O))
		sceneRenderer.togglePerspectiveFix();
	if (input.getKeyDown(GLFW_KEY_I))
		fb.toggleDepthBuffering();
	if (input.getKeyDown(GLFW_KEY_U))
		sceneRenderer.toggleWireframe();
	if (input.getKeyDown(GLFW_KEY_R))
		selectObjectInFrontOfCamera();
	if (input.getKeyDown(GLFW_KEY_X))
		deleteSelectedObject();
	if (input.getKeyDown(GLFW_KEY_C))
		showCreateScreen();
}
void Editor::moveCamera(float deltaTime)
{
	Camera& camera = scene->getMainCamera();
	const glm::vec3& forward = camera.GetForward();
	const glm::vec3& up = camera.GetUp();
	glm::vec3 pos = camera.GetPosition();
	glm::vec3 right = glm::cross(forward, up);
	if (input.getKey(GLFW_KEY_W))
		pos += forward * deltaTime;
	if (input.getKey(GLFW_KEY_A))
		pos -= right * deltaTime;
	if (input.getKey(GLFW_KEY_S))
		pos -= forward * deltaTime;
	if (input.getKey(GLFW_KEY_D))
		pos += right * deltaTime;
	if (input.getKey(GLFW_KEY_Q))
		pos += up * deltaTime;
	if (input.getKey(GLFW_KEY_E))
		pos -= up * deltaTime;
	camera.LookAt(pos, forward, up);
}

void Editor::rotateCamera()
{
	glm::vec2 mouseDiff = input.getMouseDiff();
	Camera& camera = scene->getMainCamera();
	glm::vec3 forward = camera.GetForward();
	const glm::vec3& up = camera.GetUp();
	mouseDiff /= glm::vec2({fb.getWidth(), fb.getHeight()});
	if (abs(mouseDiff.y) > 0.001)
	{
		glm::vec3 right = glm::cross(forward, up);
		forward = TransformationMatrices::getRotationMatrix(-(float)mouseDiff.y * 0.6f, right)
			* glm::vec4(forward, 1);

	}
	if (abs(mouseDiff.x) > 0.001)
	{
		forward = TransformationMatrices::getRotationMatrix(-(float)mouseDiff.x * 1.6f, up)
			* glm::vec4(forward, 1);
	}
	camera.LookAt(camera.GetPosition(), forward, up);
}

void Editor::selectObjectInFrontOfCamera()
{
	Raycast raycast(*scene);
	Camera& camera = scene->getMainCamera();
	selectedObject = raycast.castRay(camera.GetPosition(), camera.GetForward());
	sceneRenderer.selectObject(selectedObject);
}

void Editor::deleteSelectedObject()
{
	if (selectedObject != nullptr)
	{
		std::vector<SceneObject *>& sceneObjects = scene->GetSceneObjects();
		sceneObjects.erase(
			std::remove(sceneObjects.begin(), sceneObjects.end(), selectedObject), sceneObjects.end());
		selectedObject = nullptr;
	}
}

void Editor::showCreateScreen()
{
	currentScreen = new CreateObjectScreen(
		[this](SceneObject* res)->void {this->createdCallback(res); }, &scene->getMainCamera());
	guiController.addDisplayable(*currentScreen);
}

void Editor::createdCallback(SceneObject* createdObject)
{
	if (createdObject != nullptr)
	{
		scene->AddSceneObject(createdObject);
	}
	guiController.RemoveDisplayable(currentScreen);
	delete currentScreen;
	currentScreen = nullptr;
}
