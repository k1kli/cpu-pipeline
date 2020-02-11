#pragma once
#include "Displayable.h"
class Label :
	public Displayable
{
public:
	Label(glm::uvec2 posInParent, std::string text, int height, int width = 0, int color = 0xFF000000);
	virtual void onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const;
	void setColor(int color);
	void setText(std::string newText);
	std::string getText();
protected:
	std::string text;
private:
	int color;
};

