#include "ListScreen.h"
#include <sstream>
#include <iomanip>

ListScreen::ListScreen(std::function<void()> doneCallback, const Scene & scene, const void * selectedObject)
	:doneCallback(doneCallback), scene(scene), selectedObject(selectedObject)
{
	createLabels();
	this->addChild(scrollablePanel);
}

void ListScreen::handleInput(const Input& input)
{
	if (input.getKeyDown(GLFW_KEY_TAB))
	{
		doneCallback();
	}
	float scroll = input.getScroll();
	glm::vec2 posInParent = listPanel.getPosInParent();
	posInParent.y = glm::min(-1.0f, (posInParent.y + scroll * 20));
	listPanel.setPosInParent(posInParent);
}

void ListScreen::onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const
{
	Panel::onPaint(guiUtils, startX, startY, width, height);
}

void ListScreen::createLabels()
{
	int y = -40;
	labels.push_back(Label({ 30, y }, std::string("Press tab again to leave"), 30));
	y -= 40;
	labels.push_back(Label({ 30, y }, std::string("Cameras"), 30));
	y -= 40;
	std::ostringstream ss;
	glm::vec3 mainCameraPos = scene.getMainCamera().GetPosition();
	for (const Camera* cam : scene.getCameras())
	{
		ss.str(std::string());
		glm::vec3 position = cam->GetPosition();
		ss << std::setprecision(3) << "at (" << position.x << ", " << position.y << ", " << position.z << ")";
		if (cam == &scene.getMainCamera())
		{
			ss << " (main)";
		}
		else
		{
			ss << std::setprecision(3) << " (" << glm::length(mainCameraPos - position) << " units away)";
		}
		labels.push_back(Label({ 20, y }, ss.str(), 20));
		y -= 30;
	}
	y -= 20;
	labels.push_back(Label({ 30, y }, std::string("Lights"), 30));
	y -= 40;
	for (const Light* light : scene.GetLights())
	{
		ss.str(std::string());
		glm::vec3 position = light->getPosition();
		ss << std::setprecision(3) << "at (" << position.x << ", " << position.y << ", " << position.z << ")"
			<< " (" << glm::length(mainCameraPos - position) << " units away)";
		if (light == selectedObject)
		{
			ss << " --selected--";
		}
		labels.push_back(Label({ 20, y }, ss.str(), 20));
		y -= 30;
	}
	y -= 20;
	labels.push_back(Label({ 30, y }, std::string("Objects"), 30));
	y -= 40;
	for (const SceneObject* sceneObject : scene.GetSceneObjects())
	{
		ss.str(std::string());
		glm::vec3 position = sceneObject->GetTransform().getPosition();
		ss << sceneObject->GetMesh().getGenerator().getName();
		ss << std::setprecision(3) << " at (" << position.x << ", " << position.y << ", " << position.z << ")"
			<< " (" << glm::length(mainCameraPos - position) << " units away)";
		if (sceneObject == selectedObject)
		{
			ss << " --selected--";
		}
		labels.push_back(Label({ 20, y }, ss.str(), 20));
		y -= 30;
	}
	listPanel = Panel(RGBA(255, 255, 255, 0), { 0,y }, -1, -y);
	scrollablePanel.addChild(listPanel);
	for (Label& label : labels)
	{
		listPanel.addChild(label);
	}
}
