#pragma once
#include "Screen.h"
#include <functional>
#include "Label.h"
class HelpScreen :
	public Screen
{
public:
	HelpScreen(std::function<void()> doneCallback);
	void handleInput(const Input& input);
private:
	Panel helpPanel = Panel(RGBA(255, 255, 255, 100), { 0,0 }, -1, -1);
	const int renderingStart = -40;
	const int editingStart = renderingStart - 170;
	const int cameraStart = editingStart - 200;
	std::vector<Label> labels =
	{
		Label({ 10,10 }, "press H to hide help", 30),
		Label({ 10,renderingStart }, "Rendering", 30),
		Label({ 10,renderingStart -40 }, "press U to toggle wireframe", 20),
		Label({ 10,renderingStart -70 }, "press I to toggle depth buffering", 20),
		Label({ 10,renderingStart -100 }, "press O to toggle perspective correction", 20),
		Label({ 10,renderingStart -130 }, "press P to toggle backface culling", 20),
		Label({ 10,editingStart }, "Editing", 30),
		Label({ 10,editingStart -40 }, "press R to select object in front of the camera", 20),
		Label({ 10,editingStart -70 }, "press X to delete selected object", 20),
		Label({ 10,editingStart -100 }, "press C to create new object in front of the camera", 20),
		Label({ 10,editingStart -130 }, "press V to edit selected object", 20),
		Label({ 10,editingStart - 160 }, "press L to select nearest light", 20),
		Label({ 10,cameraStart }, "Camera", 30),
		Label({ 10,cameraStart -40 }, "press WASD to move", 20),
		Label({ 10,cameraStart -70 }, "press Q/E to go up/down", 20),
		Label({ 10,cameraStart -100 }, "use scroll wheel to adjust fov", 20),
		Label({ -650,-40 }, "controls for free view, other screens explain their controls", 20),
	};
	std::function<void()> doneCallback;
};

