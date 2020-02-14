#include "CreateObjectScreen.h"
#include "CuboidMeshGenerator.h"
#include <Camera.h>
#include <memory>
#include "SphereMeshGenerator.h"
#include "CyllinderMeshGenerator.h"
#include "ConeMeshGenerator.h"

CreateObjectScreen::CreateObjectScreen(std::function<void(SceneObject*)> doneCallback, const Camera * currentCamera)
	:doneCallback(doneCallback), currentCamera(currentCamera)
{
	this->addChild(sidePanel);
	sidePanel.addChild(chooseMeshLabel);
	sidePanel.addChild(pressNumberLabel);
	sidePanel.addChild(cubeLabel);
	sidePanel.addChild(sphereLabel);
	sidePanel.addChild(cyllinderLabel);
	sidePanel.addChild(coneLabel);
	sidePanel.addChild(pressCAgainToLeave);

}

void CreateObjectScreen::handleInput(const Input& input)
{
	if (input.getKey(GLFW_KEY_1))
	{
		createObject(CuboidMeshGenerator(1.0f,1.0f,1.0f));
	}
	if (input.getKey(GLFW_KEY_2))
	{
		createObject(SphereMeshGenerator(0.5f, 10, 10));
	}
	if (input.getKey(GLFW_KEY_3))
	{
		createObject(CyllinderMeshGenerator(1.0f, 0.5f, 10));
	}
	if (input.getKey(GLFW_KEY_4))
	{
		createObject(ConeMeshGenerator(1.0f, 0.5f, 10));
	}
	if (input.getKeyDown(GLFW_KEY_C))
	{
		doneCallback(nullptr);
	}
}

void CreateObjectScreen::createObject(VirtualMeshGenerator & meshGenerator)
{
	Material defaultMaterial = Material({ 0.0f,0.0f,0.0f }, { 0.1f,0.1f,0.1f }, 1.0f,
		StaticColorSampler(glm::vec3({ 1.0f,1.0f,1.0f })), StaticColorSampler(glm::vec3({ 0.0f,0.0f,1.0f })));
	Mesh mesh = meshGenerator.getMesh();
	Transform transform;
	transform.SetPosition(currentCamera->GetPosition() + currentCamera->GetForward()*3.0f);
	
	doneCallback(new SceneObject(meshGenerator.getMesh(), defaultMaterial, transform));
}
