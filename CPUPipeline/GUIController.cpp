#include "GUIController.h"


GUIController::GUIController(FrameBuffer& fb) :guiUtils(fb), masterPanel(
	RGBA(0, 0, 0, 0), { 0,0 }, -1, -1)
{
}

void GUIController::Render()
{
	masterPanel.Display(guiUtils, 0, 0, guiUtils.fb.getWidth(), guiUtils.fb.getHeight());
}

void GUIController::addDisplayable(Displayable& displayable)
{
	masterPanel.addChild(displayable);
}
void GUIController::removeDisplayable(const Displayable * displayable)
{
	masterPanel.removeChild(displayable);
}
