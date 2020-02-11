#pragma once
#include "Screen.h"
#include <functional>
#include "SceneObject.h"
#include "Label.h"
#include "VirtualMeshGenerator.h"
#include "Material.h"
#include "StaticColorSampler.h"
#include "Camera.h"
class CreateObjectScreen :
	public Screen
{
public:
	CreateObjectScreen(std::function<void (SceneObject*)> doneCallback, const Camera* currentCamera);
	void handleInput(const Input& input);
private:
	std::function<void(SceneObject*)> doneCallback;
	const Camera* currentCamera;

	Panel sidePanel = Panel(RGBA(255, 255, 255, 100), { 0,0 }, 300, -1);
	Label chooseMeshLabel = Label({ 30, -160 }, std::string("Choose mesh"), 30);
	Label pressNumberLabel = Label({ 30, -180 }, std::string("press corresponding number"), 15);
	Label cubeLabel = Label({ 30, -220 }, std::string("1. cube"), 25);
	Label sphereLabel = Label({ 30, -260 }, std::string("2. sphere"), 25);
	Label cyllinderLabel = Label({ 30, -300 }, std::string("3. cyllinder"), 25);
	Label coneLabel = Label({ 30, -340 }, std::string("4. cone"), 25);

	void createMesh(VirtualMeshGenerator & meshGenerator);
};

