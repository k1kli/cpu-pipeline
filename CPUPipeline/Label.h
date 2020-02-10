#pragma once
#include "Displayable.h"
class Label :
	public Displayable
{
public:
	Label(glm::uvec2 posInParent, std::string text, int height);
	void onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const;
	void setColor(int color);
private:
	std::string text;
	int color = 0xFF000000;
};

