#pragma once
#include <FrameBuffer.h>
class Displayable
{
public:
	Displayable(glm::uvec2 posInParent);
	void setPosInParent(glm::uvec2 posInParent);
	void setPrefferedSize(glm::uvec2 prefferedSize);
	void Display(FrameBuffer& fb, int startX, int startY, int parentWidth, int parentHeight);
	virtual void onPaint(FrameBuffer& fb, int startX, int startY, int width, int height) const = 0;
	glm::uvec2 getPosInParent() const;
private:
	glm::uvec2 posInParent;
	glm::uvec2 prefferedSize = { -1,-1 };
};

