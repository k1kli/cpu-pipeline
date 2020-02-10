#pragma once
#include <FrameBuffer.h>
#include "TextDrawer.h"
class GUIUtils
{
public:
	GUIUtils(FrameBuffer& fb) :fb(fb), textDrawer(fb) {}
	FrameBuffer& fb;
	TextDrawer textDrawer;
};

