#include "Displayable.h"


Displayable::Displayable(glm::uvec2 posInParent)
{
	setPosInParent(posInParent);
}

void Displayable::setPosInParent(glm::uvec2 posInParent)
{
	this->posInParent = posInParent;
}

void Displayable::setPrefferedSize(glm::uvec2 prefferedSize)
{
	this->prefferedSize = prefferedSize;
}

void Displayable::Display(GUIUtils& guiUtils, int startX, int startY, int parentWidth, int parentHeight)
{
	int width = prefferedSize.x;
	int height = prefferedSize.y;
	if (startX < 0)
		startX += parentWidth;
	if (startY < 0)
		startY += parentHeight;
	if (prefferedSize.x == -1)
		width = parentWidth - startX;
	if (prefferedSize.y == -1)
		height = parentHeight - startY;
	onPaint(guiUtils, startX, startY, width, height);
}


glm::uvec2 Displayable::getPosInParent() const
{
	return posInParent;
}
