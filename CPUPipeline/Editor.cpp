#include "Editor.h"
#include <TransformationMatrices.h>
#include "Raycast.h"
#include <algorithm>
#include "CreateObjectScreen.h"
#include "EditObjectScreen.h"
#include "HelpScreen.h"
#include "EditLightScreen.h"
#include <sstream>
#include <iomanip>

void Editor::handleInput(float deltaTime)
{
	if(input.getKey(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	if (currentScreen != nullptr)
	{
		currentScreen->handleInput(input);
		return;
	}
	std::ostringstream ss;
	glm::vec3 pos = scene->getMainCamera().GetPosition();
	ss << std::setprecision(3) << "You are currently at (" << pos.x << ", " << pos.y << ", " << pos.z << ")";
	positionLabel.setText(ss.str());
	moveCamera(deltaTime);
	rotateCamera();
	updateCameraClippingPlanesAndFov();
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
	{
		deleteSelectedObject();
		deleteSelectedLight();
	}
	if (input.getKeyDown(GLFW_KEY_C))
		showCreateScreen();
	if (input.getKeyDown(GLFW_KEY_V))
	{
		showEditObjectScreen();
		showEditLightScreen();
	}
	if (input.getKeyDown(GLFW_KEY_H))
		showHelpScreen();
	if (input.getKeyDown(GLFW_KEY_L))
		selectNearestLight();
	if (input.getKeyDown(GLFW_KEY_K))
		addLight();
	if (input.getKeyDown(GLFW_KEY_G))
		createNewCamera();
	if (input.getKeyDown(GLFW_KEY_F))
		swapToNextCamera();
	if (input.getKeyDown(GLFW_KEY_J))
	{
		deleteCurrentCamera();
	}
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

void Editor::updateCameraClippingPlanesAndFov()
{
	float nearP = scene->getMainCamera().GetNearPlane();
	float farP = scene->getMainCamera().GetFarPlane();
	float fov = scene->getMainCamera().GetFov();
	if (input.getKey(GLFW_KEY_LEFT_SHIFT))
	{
		if (input.getKey(GLFW_KEY_COMMA))
		{
			nearP = nearP * 0.9f;
		}
		else if (input.getKey(GLFW_KEY_PERIOD))
		{
			nearP = nearP * 1.1f;
		}
	}
	else
	{
		if (input.getKey(GLFW_KEY_COMMA))
		{
			farP = farP * 0.9f;
		}
		else if (input.getKey(GLFW_KEY_PERIOD))
		{
			farP = farP * 1.1f;
		}
	}
	fov = fov + input.getScroll();
	scene->getMainCamera().SetViewport(0, 0, (float)fb.getWidth(), (float)fb.getHeight());
	scene->getMainCamera().SetPerspective(fov, (float)fb.getHeight() / (float)fb.getWidth(), nearP, farP);
}

void Editor::createNewCamera()
{
	Camera camera = scene->getMainCamera();
	camera.LookAt(camera.GetPosition() + camera.GetForward(), camera.GetForward(), camera.GetUp());
	scene->AddCamera(camera);
}

void Editor::swapToNextCamera()
{
	for (int i = 0; i < scene->getCameras().size(); i++)
	{
		if (scene->getCameras()[i] == &scene->getMainCamera())
		{
			i = (i + 1) % scene->getCameras().size();
			scene->SetMainCamera(i);
		}
	}
}

void Editor::deleteCurrentCamera()
{
	std::vector<Camera*>& cameras = scene->getCameras();
	Camera * oldCamera = &scene->getMainCamera();
	swapToNextCamera();
	if (cameras.size() > 1)
	{
		cameras.erase(std::remove(cameras.begin(), cameras.end(), oldCamera), cameras.end());
		delete oldCamera;
	}
}


void Editor::selectObjectInFrontOfCamera()
{
	deselect();
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
		delete selectedObject;
		selectedObject = nullptr;
	}
}void Editor::deleteSelectedLight()
{
	if (selectedLight != nullptr)
	{
		std::vector<Light*>& lights = scene->GetLights();
		lights.erase(
			std::remove(lights.begin(), lights.end(), selectedLight), lights.end());
		delete selectedLight;
		selectedLight = nullptr;
	}
}

void Editor::showCreateScreen()
{
	guiController.removeDisplayable(&defaultPanel);
	currentScreen = new CreateObjectScreen(
		[this](SceneObject* res)->void {this->createdCallback(res); }, &scene->getMainCamera());
	guiController.addDisplayable(*currentScreen);
}
void Editor::showEditObjectScreen()
{
	if (selectedObject == nullptr) return;
	guiController.removeDisplayable(&defaultPanel);
	currentScreen = new EditObjectScreen(
		[this]()->void {this->defaultScreenCallback(); }, *selectedObject, scene->getImageStorage());
	guiController.addDisplayable(*currentScreen);
}

void Editor::showHelpScreen()
{
	guiController.removeDisplayable(&defaultPanel);
	currentScreen = new HelpScreen(
		[this]()->void {this->defaultScreenCallback(); });
	guiController.addDisplayable(*currentScreen);
}

void Editor::createdCallback(SceneObject* createdObject)
{
	if (createdObject != nullptr)
	{
		scene->addSceneObject(createdObject);
	}
	defaultScreenCallback();
}

void Editor::defaultScreenCallback()
{
	guiController.removeDisplayable(currentScreen);
	delete currentScreen;
	currentScreen = nullptr;
	guiController.addDisplayable(defaultPanel);
}

void Editor::selectNearestLight()
{
	deselect();
	float minDist = INFINITY;
	Light* closest = nullptr;
	glm::vec3 cameraPos = scene->getMainCamera().GetPosition();
	for (Light* light : scene->GetLights())
	{
		glm::vec3 lightPos = light->getPosition();
		float dist = glm::length(lightPos - cameraPos);
		if (dist < minDist)
		{
			minDist = dist;
			closest = light;
		}
	}
	selectedLight = closest;
	sceneRenderer.selectLight(closest);
}

void Editor::deselect()
{
	selectedLight = nullptr;
	selectedObject = nullptr;
}

void Editor::addLight()
{
	glm::vec3 pos = scene->getMainCamera().GetPosition() + scene->getMainCamera().GetForward();
	scene->AddLight(Light(pos, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
		1.0f, 0.09f, 0.032f));
}

void Editor::showEditLightScreen()
{
	if (selectedLight == nullptr) return;
	guiController.removeDisplayable(&defaultPanel);
	currentScreen = new EditLightScreen(
		[this]()->void {this->defaultScreenCallback(); }, *selectedLight);
	guiController.addDisplayable(*currentScreen);
}
