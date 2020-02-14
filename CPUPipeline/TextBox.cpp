#include "TextBox.h"

TextBox::TextBox(glm::vec2 posInParent, std::string text, int height, int width, int maxChars)
	:Label(posInParent, "", height, width, RGB(0,0,0)), maxChars(maxChars)
{
	setText(text.length() >= maxChars ?
		text.substr(0, maxChars) : text);
	setSelected(false);
}
void TextBox::onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const
{
	guiUtils.fb.DrawRect(startX, startY, startX+width, startY+height,
	selected ? RGB(75,0,130) : RGB(255,255,255));
	Label::onPaint(guiUtils, startX, startY, width, height);
}

void TextBox::handleInput(const Input& input)
{
	for (char c : input.getCharactersTyped())
	{
		if (text.length() + 1 < maxChars)
		{
			text = text + c;
		}
	}
	if (input.getKeyDown(GLFW_KEY_BACKSPACE)&&text.length() > 0)
	{
		text = text.substr(0, text.length() - 1);
	}
}

void TextBox::setSelected(bool newSelected)
{
	selected = newSelected;
	setColor(!selected ? RGB(75, 0, 130) : RGB(255, 255, 255));
}
