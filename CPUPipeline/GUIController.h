#pragma once
#include <FrameBuffer.h>
#include "Displayable.h"
#include "Panel.h"
class GUIController
{
public:
	GUIController(FrameBuffer& fb);
	void Render();
	void addDisplayable(Displayable& displayable);
private:
	FrameBuffer& fb;
	Panel masterPanel;
};

