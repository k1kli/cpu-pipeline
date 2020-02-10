#pragma once
#include "SceneRenderer.h"
#include "GUIController.h"
#include "Label.h"
class Editor
{
public:
	Editor(GUIController& guiController, SceneRenderer& sceneRenderer, Scene* scene)
		:guiController(guiController), sceneRenderer(sceneRenderer), scene(scene),
		defaultHelpLabel({ 0,0 }, "press H for help", 30)
	{
		defaultHelpLabel.setColor(RGB(255, 255, 255));
		guiController.addDisplayable(defaultHelpLabel);
	}

private:
	GUIController& guiController;
	SceneRenderer& sceneRenderer;
	Scene* scene;
	Label defaultHelpLabel;
};

