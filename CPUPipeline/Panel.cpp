#include "Panel.h"

Panel::Panel(int color, glm::uvec2 posInParent, int width, int height)
	:Displayable(posInParent), color(color)
{
	setPrefferedSize({ width, height });
}

void Panel::onPaint(GUIUtils& guiUtils, int x, int y, int width, int height) const
{
	guiUtils.fb.DrawRect(x, y, x + width, y + height, color);
	for (Displayable* child : children)
	{
		glm::uvec2 childPosInParent = child->getPosInParent();
		child->Display(guiUtils, x + childPosInParent.x, y + childPosInParent.y, width, height);
	}
}

void Panel::addChild(Displayable& child)
{
	children.push_back(&child);
}
void Panel::removeChild(const Displayable* child)
{
	for (auto it = children.begin(); it < children.end(); it++)
	{
		if (*it == child)
		{
			children.erase(it);
			return;
		}
	}
}
