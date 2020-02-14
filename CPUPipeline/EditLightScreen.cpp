#include "EditLightScreen.h"
#include <sstream>

EditLightScreen::EditLightScreen(std::function<void()> doneCallback, Light& lightToModify)
	:doneCallback(doneCallback), lightToModify(lightToModify)
{
	this->addChild(leftSidePanel);
	loadEditor();
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}

void EditLightScreen::handleInput(const Input& input)
{
	if (input.getKeyDown(GLFW_KEY_V))
	{
		doneCallback();
	}
	else if (input.getKeyDown(GLFW_KEY_TAB))
	{
		if (input.getKey(GLFW_KEY_LEFT_SHIFT))
			selectPreviousParameter();
		else
			selectNextParameter();
	}
	else if (input.getKeyDown(GLFW_KEY_ENTER))
	{
		tryApply();
	}
	else
	{
		parameterValuesTextBoxes[selectedParameterId]->handleInput(input);
	}
}

EditLightScreen::~EditLightScreen()
{
}

void EditLightScreen::selectNextParameter()
{
	parameterValuesTextBoxes[selectedParameterId]->setSelected(false);
	selectedParameterId = (selectedParameterId + 1) % parameterNamesLabels.size();
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}

void EditLightScreen::selectPreviousParameter()
{
	parameterValuesTextBoxes[selectedParameterId]->setSelected(false);
	selectedParameterId = (selectedParameterId - 1);
	if (selectedParameterId < 0) selectedParameterId += parameterValuesTextBoxes.size();
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}

void EditLightScreen::tryApply()
{
	glm::vec3 specular;
	specular.x = stof(parameterValuesTextBoxes[0]->getText());
	specular.y = stof(parameterValuesTextBoxes[1]->getText());
	specular.z = stof(parameterValuesTextBoxes[2]->getText());
	glm::vec3 diffuse;
	diffuse.x = stof(parameterValuesTextBoxes[3]->getText());
	diffuse.y = stof(parameterValuesTextBoxes[4]->getText());
	diffuse.z = stof(parameterValuesTextBoxes[5]->getText());
	float attenuationConstant = stof(parameterValuesTextBoxes[6]->getText());
	float attenuationLinear = stof(parameterValuesTextBoxes[7]->getText());
	float attenuationQuadratic = stof(parameterValuesTextBoxes[8]->getText());
	glm::vec3 position;
	position.x = stof(parameterValuesTextBoxes[9]->getText());
	position.y = stof(parameterValuesTextBoxes[10]->getText());
	position.z = stof(parameterValuesTextBoxes[11]->getText());
	lightToModify = Light(position, diffuse, specular,
		attenuationConstant, attenuationLinear, attenuationQuadratic);

	loadEditor();
}

void EditLightScreen::loadEditor()
{
	leftSidePanel.removeAllChildren();
	parameterNamesLabels.clear();
	parameterValuesTextBoxes.clear();
	leftSidePanel.addChild(pressEnterToApply);
	leftSidePanel.addChild(pressTabToJumpToNextField);
	leftSidePanel.addChild(pressVAgainToLeave);
	int y = -40;
	editLightLabel.setPosInParent({ editLightLabel.getPosInParent().x, y });
	leftSidePanel.addChild(editLightLabel);
	y -= 40;
	specularLabel.setPosInParent({specularLabel.getPosInParent().x, y});
	leftSidePanel.addChild(specularLabel);
	y -= 40;
	addEditorField(y, "R (0-1)", lightToModify.getSpecularColor().r, leftSidePanel);
	addEditorField(y, "G (0-1)", lightToModify.getSpecularColor().g, leftSidePanel);
	addEditorField(y, "B (0-1)", lightToModify.getSpecularColor().b, leftSidePanel);

	y -= 20;
	diffuseLabel.setPosInParent({ diffuseLabel.getPosInParent().x, y });
	leftSidePanel.addChild(diffuseLabel);
	y -= 40;
	addEditorField(y, "R (0-1)", lightToModify.getDiffuseColor().r, leftSidePanel);
	addEditorField(y, "G (0-1)", lightToModify.getDiffuseColor().g, leftSidePanel);
	addEditorField(y, "B (0-1)", lightToModify.getDiffuseColor().b, leftSidePanel);

	y -= 20;
	attenuationLabel.setPosInParent({ attenuationLabel.getPosInParent().x, y });
	leftSidePanel.addChild(attenuationLabel);
	y -= 40;
	addEditorField(y, "constant", lightToModify.getAttenuationConstant(), leftSidePanel);
	addEditorField(y, "linear", lightToModify.getAttenuationLinear(), leftSidePanel);
	addEditorField(y, "quadratic", lightToModify.getAttenuationQuadratic(), leftSidePanel);

	y -= 20;
	positionLabel.setPosInParent({ positionLabel.getPosInParent().x, y });
	leftSidePanel.addChild(positionLabel);
	y -= 40;
	addEditorField(y, "X", lightToModify.getPosition().x, leftSidePanel);
	addEditorField(y, "Y", lightToModify.getPosition().y, leftSidePanel);
	addEditorField(y, "Z", lightToModify.getPosition().z, leftSidePanel);
}

void EditLightScreen::addEditorField(int& y, std::string name, float value, Panel& panel)
{
	std::ostringstream ss;
	parameterNamesLabels.push_back(new Label({ 30, y }, name, 20));
	ss << value;
	parameterValuesTextBoxes.push_back(new TextBox({ -120, y }, ss.str(), 20, 100, 8));
	panel.addChild(*parameterNamesLabels[parameterNamesLabels.size() - 1]);
	panel.addChild(*parameterValuesTextBoxes[parameterNamesLabels.size() - 1]);
	y -= 30;
}

