#pragma once
#include "Screen.h"
#include <functional>
#include "Scene.h"
#include "Label.h"
class ListScreen :
	public Screen
{
public:
	ListScreen(std::function<void()> doneCallback, const Scene& scene, const void* selectedObject);
	void handleInput(const Input& input);
protected:
	virtual void onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const;
private:
	std::function<void()> doneCallback;
	const Scene& scene;
	const void* selectedObject;
	Panel listPanel = Panel(RGBA(255, 255, 255, 0), { 0,0 }, -1, -1);
	Panel scrollablePanel = Panel(RGBA(255, 255, 255, 100), { 0,0 }, -1, -1);
	std::vector<Label> labels;
	void createLabels();
};

