#include "Displayable.h"


Displayable::Displayable(glm::vec2 posInParent)
{
	setPosInParent(posInParent);
}

void Displayable::setPosInParent(glm::vec2 posInParent)
{
	this->posInParent = posInParent;
}

void Displayable::setPrefferedSize(glm::vec2 prefferedSize)
{
	this->prefferedSize = prefferedSize;
}

void Displayable::Display(GUIUtils& guiUtils, int startX, int startY, int parentWidth, int parentHeight)
{
	int width = prefferedSize.x;
	int height = prefferedSize.y;
	if (getPosInParent().x < 0)
		startX += parentWidth;
	if (getPosInParent().y < 0)
		startY += parentHeight;
	if (prefferedSize.x < 0)
		width = parentWidth - startX;
	if (prefferedSize.y < 0)
		height = parentHeight - startY;
	onPaint(guiUtils, startX, startY, width, height);
}


glm::vec2 Displayable::getPosInParent() const
{
	return posInParent;
}
