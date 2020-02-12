#pragma once
#include "Displayable.h"
class Panel :
	public Displayable
{
public:
	Panel(int color, glm::vec2 posInParent, int width, int height);
	virtual void onPaint(GUIUtils& guiUtils, int x, int y, int width, int height) const;
	void addChild(Displayable& child);
	void removeChild(const Displayable* child);
	void removeAllChildren();
private:
	int color;
	std::vector<Displayable *> children;
};

