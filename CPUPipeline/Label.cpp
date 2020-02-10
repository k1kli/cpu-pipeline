#include "Label.h"

Label::Label(glm::uvec2 posInParent, std::string text, int height) :Displayable(posInParent), text(text)
{
	setPrefferedSize({ 0,height });
}

void Label::onPaint(GUIUtils& guiUtils, int startX, int startY, int width, int height) const
{
	guiUtils.textDrawer.DrawTextAt(text, startX, startY, color, height);
}
