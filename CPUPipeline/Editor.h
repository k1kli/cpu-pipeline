#pragma once
#include "SceneRenderer.h"
#include "GUIController.h"
#include "Label.h"
#include "Input.h"
#include "Screen.h"
class Editor
{
public:
	Editor(GUIController& guiController, SceneRenderer& sceneRenderer, Scene* scene, const Input & input,
		FrameBuffer & fb, GLFWwindow * window)
		:guiController(guiController), sceneRenderer(sceneRenderer), scene(scene),
		defaultHelpLabel({ 10,10 }, "press H for help", 30), input(input), fb(fb), window(window)
	{

		defaultHelpLabel.setColor(RGB(255, 255, 255));
		guiController.addDisplayable(defaultHelpLabel);
	}
	void handleInput(float deltaTime);
private:
	GUIController& guiController;
	SceneRenderer& sceneRenderer;
	Scene* scene;
	Label defaultHelpLabel;
	GLFWwindow* window;
	FrameBuffer & fb;
	const Input& input;
	SceneObject* selectedObject = nullptr;
	Screen* currentScreen = nullptr;

	void moveCamera(float deltaTime);
	void rotateCamera();
	void selectObjectInFrontOfCamera();
	void deleteSelectedObject();
	void showCreateScreen();
	void showEditObjectScreen();
	void createdCallback(SceneObject* createdObject);
	void defaultScreenCallback();
};

