#pragma once
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <vector>
class Input
{
public:
	Input();
	void setMousePos(double newX, double newY);
	void setWindowDim(double newWidth, double newHeight);
	void updateKeyboardInput();
	void updateMouseInput();
	void setKeyPressed(int key);
	void setKeyReleased(int key);
	void characterTyped(char c);
	const glm::vec2 & getMousePos() const;
	const glm::vec2 & getMouseDiff() const;
	const bool getKeyDown(int keyCode) const;
	const bool getKey(int keyCode) const;
	const std::vector<char>& getCharactersTyped() const;
private:
	glm::vec2 mousePos = { 0,0 };
	glm::vec2 mouseDiff = { 0,0 };
	glm::vec2 newMousePos = { 0,0 };
	double currentWidth = 0;
	double currentHeight = 0;
	bool pressedThisFrame[GLFW_KEY_LAST + 1];
	bool pressedPreviousFrame[GLFW_KEY_LAST + 1];
	std::vector<char> charactersReceivedThisFrame;
};

