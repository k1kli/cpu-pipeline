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
	std::vector<Label> labels =
	{
		Label({ 10,10 }, "press H to hide help", 30),
		Label({ 10,-40 }, "Rendering", 30),
		Label({ 10,-70 }, "press U to toggle wireframe", 20),
		Label({ 10,-110 }, "press I to toggle depth buffering", 20),
		Label({ 10,-140 }, "press O to toggle perspective correction", 20),
		Label({ 10,-170 }, "press P to toggle backface culling", 20),
		Label({ 10,-220 }, "Editing", 30),
		Label({ 10,-260 }, "press R to select object in front of the camera", 20),
		Label({ 10,-290 }, "press X to delete selected object", 20),
		Label({ 10,-320 }, "press C to create new object in front of the camera", 20),
		Label({ 10,-350 }, "press V to edit selected object", 20),
		Label({ 10,-390 }, "Camera", 30),
		Label({ 10,-430 }, "press WASD to move", 20),
		Label({ 10,-460 }, "press Q/E to go up/down", 20),
		Label({ 10,-490 }, "use scroll wheel to adjust fov", 20),
		Label({ -650,-40 }, "controls for free view, other screens explain their controls", 20),
	};
	std::function<void()> doneCallback;
};

