#pragma once
#include <FrameBuffer.h>
#include "GUIUtils.h"
class Displayable
{
public:
	Displayable(glm::uvec2 posInParent);
	void setPosInParent(glm::uvec2 posInParent);
	void Display(GUIUtils& guiUtils, int startX, int startY, int parentWidth, int parentHeight);
	virtual void onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const = 0;
	glm::uvec2 getPosInParent() const;
protected:
	void setPrefferedSize(glm::uvec2 prefferedSize);
private:
	glm::uvec2 posInParent;
	glm::uvec2 prefferedSize = { -1,-1 };
};

