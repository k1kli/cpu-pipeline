#include "Label.h"

Label::Label(glm::vec2 posInParent, std::string text, int height, int width, int color)
	:Displayable(posInParent), text(text), color(color)
{
	setPrefferedSize({ width,height });
}

void Label::onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const
{
	guiUtils.textDrawer.DrawTextAt(text, startX, startY, color, height);
}

void Label::setColor(int color)
{
	this->color = color;
}

void Label::setText(std::string newText)
{
	text = newText;
}

std::string Label::getText()
{
	return text;
}
