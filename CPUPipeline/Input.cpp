#include "Input.h"
#include <string>


Input::Input(GLFWwindow* window):window(window)
{
	for (int i = 0; i < GLFW_KEY_LAST + 1; i++)
	{
		pressedPreviousFrame[i] = false;
		pressedThisFrame[i] = false;
	}
}

void Input::setMousePos(double newX, double newY)
{
	newMousePos = { newX - currentWidth / 2.0, newY - currentHeight / 2.0 };
}

void Input::setWindowDim(double newWidth, double newHeight)
{
	currentWidth = newWidth;
	currentHeight = newHeight;
}

void Input::updateKeyboardInput()
{
	for (int i = 0; i < GLFW_KEY_LAST + 1; i++)
	{
		pressedPreviousFrame[i] = pressedThisFrame[i];
	}
	charactersReceivedThisFrame.clear();
}
void Input::updateMouseInput()
{
	mouseDiff = newMousePos - mousePos;
	mousePos = newMousePos;
}

void Input::setKeyPressed(int key)
{
	pressedThisFrame[key] = true;
}
void Input::setKeyReleased(int key)
{
	pressedThisFrame[key] = false;
}

void Input::characterTyped(char c)
{
	charactersReceivedThisFrame.push_back(c);
}

const glm::vec2& Input::getMousePos() const
{
	return mousePos;
}

const glm::vec2& Input::getMouseDiff() const
{
	return mouseDiff;
}

const bool Input::getKeyDown(int keyCode) const
{
	return keyCode < GLFW_KEY_LAST+1 && pressedThisFrame[keyCode] && !pressedPreviousFrame[keyCode];
}

const bool Input::getKey(int keyCode) const
{
	return keyCode < GLFW_KEY_LAST + 1 && pressedThisFrame[keyCode];
}

const std::vector<char>& Input::getCharactersTyped() const
{
	return charactersReceivedThisFrame;
}

std::string Input::getClipboardString() const
{
	const char * clipboard = glfwGetClipboardString(window);
	return std::string(clipboard ? clipboard : "");
}
