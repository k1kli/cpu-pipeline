#include "CreateObjectScreen.h"
#include "CuboidMeshGenerator.h"

CreateObjectScreen::CreateObjectScreen(std::function<void(SceneObject*)> doneCallback)
	:doneCallback(doneCallback)
{
	this->addChild(sidePanel);
	sidePanel.addChild(chooseMeshLabel);
	sidePanel.addChild(pressNumberLabel);
	sidePanel.addChild(cubeLabel);
	sidePanel.addChild(sphereLabel);
	sidePanel.addChild(cyllinderLabel);
	sidePanel.addChild(coneLabel);

}

void CreateObjectScreen::handleInput(const Input& input)
{
	if (input.getKey(GLFW_KEY_1))
	{
		createMesh(CuboidMeshGenerator(1.0f,1.0f,1.0f));
	}
	if (input.getKey(GLFW_KEY_2))
	{
		createMesh(CuboidMeshGenerator(1.0f, 1.0f, 1.0f));
	}
	if (input.getKey(GLFW_KEY_3))
	{
		createMesh(CuboidMeshGenerator(1.0f, 1.0f, 1.0f));
	}
	if (input.getKey(GLFW_KEY_4))
	{
		createMesh(CuboidMeshGenerator(1.0f, 1.0f, 1.0f));
	}
	if (input.getKey(GLFW_KEY_BACKSPACE))
	{
		doneCallback(nullptr);
	}
}

void CreateObjectScreen::createMesh(VirtualMeshGenerator & meshGenerator)
{
	doneCallback(nullptr);
}
