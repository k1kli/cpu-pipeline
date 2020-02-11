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
		createMesh(CuboidMeshGenerator(1.0f,1.0f,1.0f));
	}
	if (input.getKey(GLFW_KEY_2))
	{
		createMesh(SphereMeshGenerator(0.5f, 10, 10));
	}
	if (input.getKey(GLFW_KEY_3))
	{
		createMesh(CyllinderMeshGenerator(1.0f, 0.5f, 10));
	}
	if (input.getKey(GLFW_KEY_4))
	{
		createMesh(ConeMeshGenerator(1.0f, 0.5f, 10));
	}
	if (input.getKeyDown(GLFW_KEY_C))
	{
		doneCallback(nullptr);
	}
}

void CreateObjectScreen::createMesh(VirtualMeshGenerator & meshGenerator)
{
	std::shared_ptr< Sampler> colorSampler = std::make_shared<StaticColorSampler>(glm::vec3({ 1.0f,1.0f,1.0f }));
	std::shared_ptr< Sampler> normalSampler = std::make_shared<StaticColorSampler>(glm::vec3({ 0.0f,0.0f,1.0f }));
	Material defaultMaterial = Material(0.0f, 1.0f, 0.3f, 1.0f,
		colorSampler, normalSampler);
	Mesh mesh = meshGenerator.getMesh();
	Transform transform;
	transform.SetPosition(currentCamera->GetPosition() + currentCamera->GetForward()*3.0f);
	
	doneCallback(new SceneObject(meshGenerator.getMesh(), defaultMaterial, transform));
}
