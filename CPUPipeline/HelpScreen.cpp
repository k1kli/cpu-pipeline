#include "HelpScreen.h"

HelpScreen::HelpScreen(std::function<void()> doneCallback)
	:doneCallback(doneCallback)
{
	addChild(helpPanel);
	for (Label& label : labels)
	{
		helpPanel.addChild(label);
	}
}

void HelpScreen::handleInput(const Input& input)
{
	if (input.getKeyDown(GLFW_KEY_H))
	{
		doneCallback();
	}
}
