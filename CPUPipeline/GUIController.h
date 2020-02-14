#pragma once
#include <FrameBuffer.h>
#include "Displayable.h"
#include "Panel.h"
#include "GUIUtils.h"
class GUIController
{
public:
	GUIController(FrameBuffer& fb);
	void Render();
	void addDisplayable(Displayable& displayable);
	void removeDisplayable(const Displayable* displayable);
private:
	GUIUtils guiUtils;
	Panel masterPanel;
};

