#include "GUIController.h"


GUIController::GUIController(FrameBuffer& fb) :fb(fb), masterPanel(
	RGBA(0, 0, 0, 0), { 0,0 }, -1, -1)
{
}

void GUIController::Render()
{
	masterPanel.Display(fb, 0, 0, fb.getWidth(), fb.getHeight());
}

void GUIController::addDisplayable(Displayable& displayable)
{
	masterPanel.addChild(displayable);
}
