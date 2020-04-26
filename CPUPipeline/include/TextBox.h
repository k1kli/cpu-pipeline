#pragma once
#include "Label.h"
#include "Input.h"
class TextBox:public Label
{
public:
	TextBox(glm::vec2 posInParent, std::string text, int height, int width, int maxChars);
	void handleInput(const Input& input);
	void setSelected(bool newSelected);
protected:
	virtual void onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const;
private:
	bool selected;
	int maxChars;
};

