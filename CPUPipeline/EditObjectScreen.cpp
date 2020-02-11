#include "EditObjectScreen.h"
#include <sstream>

EditObjectScreen::EditObjectScreen(std::function<void()> doneCallback, SceneObject& sceneObjectToModify)
	:doneCallback(doneCallback), sceneObjectToModify(sceneObjectToModify)
{
	this->addChild(sidePanel);
	sidePanel.addChild(editMeshLabel);
	const VirtualMeshGenerator& generator = sceneObjectToModify.GetMesh().getGenerator();
	
	const std::string& name = generator.getName();
	meshNameLabel.setText(name);
	sidePanel.addChild(meshNameLabel);
	sidePanel.addChild(pressVAgainToLeave);
	sidePanel.addChild(pressTabToJumpToNextField);
	sidePanel.addChild(pressEnterToApply);

	const std::vector<std::string>& names = generator.getParameterNames();
	const std::vector<float>& values = generator.getParameterValues();
	int y = -100;
	for (int i = 0; i < names.size(); i++, y -=40)
	{
		parameterNamesLabels.push_back(new Label({ 30, y }, names[i], 20));
		std::ostringstream ss;
		ss << values[i];
		parameterValuesTextBoxes.push_back(new TextBox({ -100, y }, ss.str(), 20, 80, 8));
		sidePanel.addChild(*parameterNamesLabels[i]);
		sidePanel.addChild(*parameterValuesTextBoxes[i]);
	}
	parameterValuesTextBoxes[0]->setSelected(true);
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
	std::shared_ptr<VirtualMeshGenerator> generator = sceneObjectToModify.GetMesh().getGenerator().getInstance();
	std::vector<float> newParameters(parameterNamesLabels.size());
	for (int i = 0; i < parameterValuesTextBoxes.size(); i++)
	{
		try
		{
			newParameters[i] = stof(parameterValuesTextBoxes[i]->getText());
		}
		catch(...)
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
