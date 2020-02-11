#include "EditObjectScreen.h"
#include <sstream>
#include "StaticColorSampler.h"

EditObjectScreen::EditObjectScreen(std::function<void()> doneCallback, SceneObject& sceneObjectToModify)
	:doneCallback(doneCallback), sceneObjectToModify(sceneObjectToModify)
{
	this->addChild(sidePanel);
	
	sidePanel.addChild(pressVAgainToLeave);
	sidePanel.addChild(pressTabToJumpToNextField);
	sidePanel.addChild(pressEnterToApply);
	sidePanel.addChild(pressNToLoadNormal);

	loadEditor();
}

void EditObjectScreen::handleInput(const Input& input)
{
	if (input.getKeyDown(GLFW_KEY_V))
	{
		doneCallback();
	}
	if (input.getKeyDown(GLFW_KEY_TAB))
	{
		selectNextParameter();
	}
	if (input.getKeyDown(GLFW_KEY_ENTER))
	{
		tryApply();
	}
	if (input.getKeyDown(GLFW_KEY_N))
	{
		loadNormalMapFromClipboard();
	}
	parameterValuesTextBoxes[selectedParameterId]->handleInput(input);
}

EditObjectScreen::~EditObjectScreen()
{
	for (int i = 0; i < parameterNamesLabels.size(); i++)
	{
		delete parameterNamesLabels[i];
		delete parameterValuesTextBoxes[i];
	}
}

void EditObjectScreen::selectNextParameter()
{
	parameterValuesTextBoxes[selectedParameterId]->setSelected(false);
	selectedParameterId = (selectedParameterId + 1) % parameterNamesLabels.size();
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}

void EditObjectScreen::tryApply()
{
	createMesh();
	createMaterial();
	loadEditor();
}

void EditObjectScreen::createMesh()
{
	std::shared_ptr<VirtualMeshGenerator> generator = sceneObjectToModify.GetMesh().getGenerator().getInstance();
	std::vector<float> newParameters(meshParametersEnd);
	for (int i = 0; i < meshParametersEnd; i++)
	{
		try
		{
			newParameters[i] = stof(parameterValuesTextBoxes[i]->getText());
		}
		catch (...)
		{
			return;
		}
	}
	generator->setParameters(newParameters);
	try
	{
		sceneObjectToModify.GetMesh() = generator->getMesh();
	}
	catch (const char* error)
	{
		return;
	}
}

void EditObjectScreen::createMaterial()
{
	Material newMaterial = sceneObjectToModify.GetMaterial();
	try
	{
		newMaterial.specular.r = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd]->getText()),0.0f,1.0f);
		newMaterial.specular.g = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 1]->getText()), 0.0f, 1.0f);
		newMaterial.specular.b = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 2]->getText()), 0.0f, 1.0f);

		newMaterial.ambient.r = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 3]->getText()), 0.0f, 1.0f);
		newMaterial.ambient.g = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 4]->getText()), 0.0f, 1.0f);
		newMaterial.ambient.b = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 5]->getText()), 0.0f, 1.0f);

		glm::vec3 diffuse;

		diffuse.r = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 6]->getText()), 0.0f, 1.0f);
		diffuse.g = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 7]->getText()), 0.0f, 1.0f);
		diffuse.b = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 8]->getText()), 0.0f, 1.0f);
		newMaterial.colorSampler = std::make_shared<StaticColorSampler>(diffuse);

		newMaterial.shininess = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 9]->getText()), 1.0f, 200.0f);

	}
	catch(...)
	{
		return;
	}
	sceneObjectToModify.SetMaterial(newMaterial);
}

void EditObjectScreen::loadEditor()
{
	int y = -100;
	parameterNamesLabels.clear();
	parameterValuesTextBoxes.clear();
	loadMeshEditor(y);
	loadMaterialEditor(y);
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}

void EditObjectScreen::loadMeshEditor(int& y)
{
	sidePanel.addChild(editMeshLabel);
	const VirtualMeshGenerator& generator = sceneObjectToModify.GetMesh().getGenerator();

	const std::vector<std::string>& names = generator.getParameterNames();
	const std::vector<float>& values = generator.getParameterValues();

	const std::string& name = generator.getName();
	meshNameLabel.setText(name);
	sidePanel.addChild(meshNameLabel);
	for (int i = 0; i < names.size(); i++, y -= 40)
	{
		parameterNamesLabels.push_back(new Label({ 30, y }, names[i], 20));
		std::ostringstream ss;
		ss << values[i];
		parameterValuesTextBoxes.push_back(new TextBox({ -100, y }, ss.str(), 20, 80, 8));
		sidePanel.addChild(*parameterNamesLabels[i]);
		sidePanel.addChild(*parameterValuesTextBoxes[i]);
	}
	meshParametersEnd = parameterNamesLabels.size();
}

void EditObjectScreen::loadMaterialEditor(int &y)
{
	const Material& oldMaterial = sceneObjectToModify.GetMaterial();
	addEditorField(y, "specular R (0-1)", oldMaterial.specular.r);
	addEditorField(y, "specular G (0-1)", oldMaterial.specular.g);
	addEditorField(y, "specular B (0-1)", oldMaterial.specular.b);

	y -= 20;

	addEditorField(y, "ambient R (0-1)", oldMaterial.ambient.r);
	addEditorField(y, "ambient G (0-1)", oldMaterial.ambient.g);
	addEditorField(y, "ambient B (0-1)", oldMaterial.ambient.b);

	if (StaticColorSampler* sts = dynamic_cast<StaticColorSampler*>(oldMaterial.colorSampler.get()))
	{
		glm::vec3 diffuse = sts->getColor();
		addEditorField(y, "diffuse R (0-1)", diffuse.r);
		addEditorField(y, "diffuse G (0-1)", diffuse.g);
		addEditorField(y, "diffuse B (0-1)", diffuse.b);
	}

	addEditorField(y, "shininess (1-200)", oldMaterial.shininess);

}

void EditObjectScreen::addEditorField(int& y, std::string name, float value)
{
	std::ostringstream ss;
	parameterNamesLabels.push_back(new Label({ 30, y }, name, 20));
	ss << value;
	parameterValuesTextBoxes.push_back(new TextBox({ -100, y }, ss.str(), 20, 80, 8));
	sidePanel.addChild(*parameterNamesLabels[parameterNamesLabels.size() - 1]);
	sidePanel.addChild(*parameterValuesTextBoxes[parameterNamesLabels.size() - 1]);
	y -= 30;
}

void EditObjectScreen::loadNormalMapFromClipboard()
{
}
