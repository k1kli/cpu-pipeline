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
		:guiController(guiController), sceneRenderer(sceneRenderer), scene(scene)
		, input(input), fb(fb), window(window)
	{
		guiController.addDisplayable(defaultHelpLabel);
	}
	void handleInput(float deltaTime);
private:
	GUIController& guiController;
	SceneRenderer& sceneRenderer;
	Scene* scene;
	Label defaultHelpLabel = Label({ 10,10 }, "press H for help", 30,0, RGB(255, 255, 255));
	GLFWwindow* window;
	FrameBuffer & fb;
	const Input& input;
	SceneObject* selectedObject = nullptr;
	Light* selectedLight = nullptr;
	Screen* currentScreen = nullptr;

	void moveCamera(float deltaTime);
	void rotateCamera();
	void selectObjectInFrontOfCamera();
	void deleteSelectedObject();
	void deleteSelectedLight();
	void showCreateScreen();
	void showEditObjectScreen();
	void showHelpScreen();
	void createdCallback(SceneObject* createdObject);
	void defaultScreenCallback();
	void selectNearestLight();
	void deselect();
};

