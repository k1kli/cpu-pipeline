#include "Panel.h"

Panel::Panel(int color, glm::uvec2 posInParent, int width, int height)
	:Displayable(posInParent), color(color)
{
	setPrefferedSize({ width, height });
}

void Panel::onPaint(FrameBuffer& fb, int x, int y, int width, int height) const
{
	fb.DrawRect(x, y, x + width, y + height, color);
	for (Displayable* child : children)
	{
		glm::uvec2 childPosInParent = child->getPosInParent();
		child->Display(fb, x + childPosInParent.x, y + childPosInParent.y, width, height);
	}
}

void Panel::addChild(Displayable& child)
{
	children.push_back(&child);
}
