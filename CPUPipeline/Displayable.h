#pragma once
#include <FrameBuffer.h>
#include "GUIUtils.h"
class Displayable
{
public:
	Displayable(glm::vec2 posInParent);
	void setPosInParent(glm::vec2 posInParent);
	void Display(GUIUtils& guiUtils, int startX, int startY, int parentWidth, int parentHeight);
	glm::vec2 getPosInParent() const;
protected:
	void setPrefferedSize(glm::vec2 prefferedSize);
	virtual void onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const = 0;
private:
	glm::vec2 posInParent;
	glm::vec2 prefferedSize = { -1,-1 };
};

