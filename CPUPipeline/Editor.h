#pragma once
#include "SceneRenderer.h"
#include "GUIController.h"
#include "Label.h"
class Editor
{
public:
	Editor(GUIController& guiController, SceneRenderer& sceneRenderer, Scene* scene, GLFWwindow* window,
		FrameBuffer & fb)
		:guiController(guiController), sceneRenderer(sceneRenderer), scene(scene),
		defaultHelpLabel({ 0,0 }, "press H for help", 30), window(window), fb(fb)
	{
		defaultHelpLabel.setColor(RGB(255, 255, 255));
		guiController.addDisplayable(defaultHelpLabel);
	}
	void processInput(float deltaTime);
	void moveCamera(float deltaTime);
	void processMouse(double xPos, double yPos);
	void rotateCamera(double xDiff, double yDiff);
private:
	GUIController& guiController;
	SceneRenderer& sceneRenderer;
	Scene* scene;
	Label defaultHelpLabel;
	GLFWwindow* window;
	FrameBuffer & fb;
	double oldXPos = 0;
	double oldYPos = 0;
	double currentWidth = 0;
	double currentHeight = 0;
};

