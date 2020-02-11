#pragma once
#include "SceneRenderer.h"
#include "GUIController.h"
#include "Label.h"
#include "Input.h"
class Editor
{
public:
	Editor(GUIController& guiController, SceneRenderer& sceneRenderer, Scene* scene, const Input & input,
		FrameBuffer & fb, GLFWwindow * window)
		:guiController(guiController), sceneRenderer(sceneRenderer), scene(scene),
		defaultHelpLabel({ 0,0 }, "press H for help", 30), input(input), fb(fb), window(window)
	{

		defaultHelpLabel.setColor(RGB(255, 255, 255));
		guiController.addDisplayable(defaultHelpLabel);
	}
	void handleInput(float deltaTime);
	void moveCamera(float deltaTime);
	void rotateCamera();
	void selectObjectInFrontOfCamera();
private:
	GUIController& guiController;
	SceneRenderer& sceneRenderer;
	Scene* scene;
	Label defaultHelpLabel;
	GLFWwindow* window;
	FrameBuffer & fb;
	const Input& input;
	SceneObject* selectedObject;
};

