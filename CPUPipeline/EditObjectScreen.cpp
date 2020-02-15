#include "EditObjectScreen.h"
#include <sstream>
#include "StaticColorSampler.h"
#include "ImageSampler.h"
#include <algorithm>

EditObjectScreen::EditObjectScreen(std::function<void()> doneCallback, SceneObject& sceneObjectToModify,
	ImageStorage & imageStorage)
	:doneCallback(doneCallback), sceneObjectToModify(sceneObjectToModify), sceneImageStorage(imageStorage)
{
	this->addChild(leftSidePanel);
	this->addChild(rightSidePanel);
	
	

	loadEditor();
}

void EditObjectScreen::handleInput(const Input& input)
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
	else if (input.getKeyDown(GLFW_KEY_N))
	{
		loadNormalMapFromClipboard(input);
	}
	else if (input.getKeyDown(GLFW_KEY_L))
	{
		loadTextureFromClipboard(input);
	}
	else if (input.getKeyDown(GLFW_KEY_M))
	{
		textureFromClipboard = ImageView();
		sceneObjectToModify.GetMaterial().setColorSampler(StaticColorSampler(glm::vec3(1.0f, 1.0f, 1.0f)));
		loadEditor();
	}
	else
	{
		parameterValuesTextBoxes[selectedParameterId]->handleInput(input);
	}
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
void EditObjectScreen::selectPreviousParameter()
{
	parameterValuesTextBoxes[selectedParameterId]->setSelected(false);
	selectedParameterId = (selectedParameterId - 1);
	if (selectedParameterId < 0) selectedParameterId += parameterValuesTextBoxes.size();
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}
void EditObjectScreen::tryApply()
{
	createTransform();
	createMesh();
	createMaterial();
	loadEditor();
}

void EditObjectScreen::createTransform()
{
	Transform newTransform = Transform();
	try
	{
		newTransform.SetPosition(glm::vec3(
			stof(parameterValuesTextBoxes[0]->getText()),
			stof(parameterValuesTextBoxes[1]->getText()),
			stof(parameterValuesTextBoxes[2]->getText())));
		newTransform.SetEulerAngles(glm::vec3(
			stof(parameterValuesTextBoxes[3]->getText()),
			stof(parameterValuesTextBoxes[4]->getText()),
			stof(parameterValuesTextBoxes[5]->getText())));
		newTransform.SetScale(glm::vec3(
			stof(parameterValuesTextBoxes[6]->getText()),
			stof(parameterValuesTextBoxes[7]->getText()),
			stof(parameterValuesTextBoxes[8]->getText())));
		sceneObjectToModify.GetTransform() = newTransform;
	}
	catch (...) {}
}

void EditObjectScreen::createMesh()
{
	std::shared_ptr<VirtualMeshGenerator> generator = sceneObjectToModify.GetMesh().getGenerator().getInstance();
	std::vector<float> newParameters(meshParametersEnd-transformParametersEnd);
	for (int i = transformParametersEnd; i < meshParametersEnd; i++)
	{
		try
		{
			newParameters[i-transformParametersEnd] = stof(parameterValuesTextBoxes[i]->getText());
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

		newMaterial.shininess = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 6]->getText()), 1.0f, 200.0f);

		if (textureFromClipboard.isValid())
		{
			newMaterial.setColorSampler(ImageSampler(textureFromClipboard));
			textureFromClipboard = ImageView();
			pressLToLoadTexture.setText("press L to load texture from clipboard filename");
		}
		else if (!diffuseFromTexture)
		{
			glm::vec3 diffuse;

			diffuse.r = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 7]->getText()), 0.0f, 1.0f);
			diffuse.g = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 8]->getText()), 0.0f, 1.0f);
			diffuse.b = glm::clamp(stof(parameterValuesTextBoxes[meshParametersEnd + 9]->getText()), 0.0f, 1.0f);
			newMaterial.setColorSampler(StaticColorSampler(diffuse));
		}

		if (normalMapFromClipboard.isValid())
		{
			newMaterial.setNormalSampler(ImageSampler(normalMapFromClipboard));
			normalMapFromClipboard = ImageView();
			pressNToLoadNormal.setText("press N to load normal map from clipboard filename");
		}



	}
	catch(...)
	{
		return;
	}
	sceneObjectToModify.SetMaterial(newMaterial);
}

void EditObjectScreen::loadEditor()
{
	leftSidePanel.removeAllChildren();
	leftSidePanel.addChild(pressVAgainToLeave);
	leftSidePanel.addChild(pressTabToJumpToNextField);
	leftSidePanel.addChild(pressEnterToApply);
	leftSidePanel.addChild(pressNToLoadNormal);
	leftSidePanel.addChild(pressLToLoadTexture);
	int y = -100;
	parameterNamesLabels.clear();
	parameterValuesTextBoxes.clear();
	loadTransformEditor();
	loadMeshEditor(y);
	loadMaterialEditor(y);
	if (selectedParameterId >= parameterNamesLabels.size())
	{
		selectedParameterId = parameterNamesLabels.size() - 1;
	}
	parameterValuesTextBoxes[selectedParameterId]->setSelected(true);
}

void EditObjectScreen::loadMeshEditor(int& y)
{
	leftSidePanel.addChild(editMeshLabel);
	const VirtualMeshGenerator& generator = sceneObjectToModify.GetMesh().getGenerator();

	const std::vector<std::string>& names = generator.getParameterNames();
	const std::vector<float>& values = generator.getParameterValues();

	const std::string& name = generator.getName();
	meshNameLabel.setText(name);
	leftSidePanel.addChild(meshNameLabel);
	for (int i = 0; i < names.size(); i++, y -= 40)
	{
		parameterNamesLabels.push_back(new Label({ 30, y }, names[i], 20));
		std::ostringstream ss;
		ss << values[i];
		parameterValuesTextBoxes.push_back(new TextBox({ -100, y }, ss.str(), 20, 80, 8));
		leftSidePanel.addChild(*parameterNamesLabels[transformParametersEnd+i]);
		leftSidePanel.addChild(*parameterValuesTextBoxes[transformParametersEnd + i]);
	}
	meshParametersEnd = parameterNamesLabels.size();
}

void EditObjectScreen::loadMaterialEditor(int &y)
{
	editMaterialLabel.setPosInParent({ editMaterialLabel.getPosInParent().x, y });
	leftSidePanel.addChild(editMaterialLabel);
	y -= 40;
	const Material& oldMaterial = sceneObjectToModify.GetMaterial();
	addEditorField(y, "specular R (0-1)", oldMaterial.specular.r, leftSidePanel);
	addEditorField(y, "specular G (0-1)", oldMaterial.specular.g, leftSidePanel);
	addEditorField(y, "specular B (0-1)", oldMaterial.specular.b, leftSidePanel);

	y -= 20;

	addEditorField(y, "ambient R (0-1)", oldMaterial.ambient.r, leftSidePanel);
	addEditorField(y, "ambient G (0-1)", oldMaterial.ambient.g, leftSidePanel);
	addEditorField(y, "ambient B (0-1)", oldMaterial.ambient.b, leftSidePanel);

	addEditorField(y, "shininess (1-200)", oldMaterial.shininess, leftSidePanel);

	if (const ImageSampler* imageSampler = dynamic_cast<const ImageSampler*>(&oldMaterial.getColorSampler()))
	{
		diffuseFromTexture = true;
		leftSidePanel.addChild(pressMToSwitchColorSampler);
		
	}
	else if (const StaticColorSampler* sts = dynamic_cast<const StaticColorSampler*>(&oldMaterial.getColorSampler()))
	{
		glm::vec3 diffuse = sts->getColor();
		addEditorField(y, "diffuse R (0-1)", diffuse.r, leftSidePanel);
		addEditorField(y, "diffuse G (0-1)", diffuse.g, leftSidePanel);
		addEditorField(y, "diffuse B (0-1)", diffuse.b, leftSidePanel);
		diffuseFromTexture = false;
	}


}

void EditObjectScreen::loadTransformEditor()
{
	Transform oldTransform = sceneObjectToModify.GetTransform();
	rightSidePanel.addChild(transformLabel);
	int y = -80;

	transformPosLabel.setPosInParent({ 30, y });
	rightSidePanel.addChild(transformPosLabel);
	y -= 30;
	addEditorField(y, "X", oldTransform.getPosition().x, rightSidePanel);
	addEditorField(y, "Y", oldTransform.getPosition().y, rightSidePanel);
	addEditorField(y, "Z", oldTransform.getPosition().z, rightSidePanel);
	y -= 30;

	transformRotLabel.setPosInParent({ 30, y });
	rightSidePanel.addChild(transformRotLabel);
	y -= 30;
	addEditorField(y, "X", oldTransform.getEulerAngles().x, rightSidePanel);
	addEditorField(y, "Y", oldTransform.getEulerAngles().y, rightSidePanel);
	addEditorField(y, "Z", oldTransform.getEulerAngles().z, rightSidePanel);
	y -= 30;

	transformScalLabel.setPosInParent({ 30, y });
	rightSidePanel.addChild(transformScalLabel);
	y -= 30;
	addEditorField(y, "X", oldTransform.getScale().x, rightSidePanel);
	addEditorField(y, "Y", oldTransform.getScale().y, rightSidePanel);
	addEditorField(y, "Z", oldTransform.getScale().z, rightSidePanel);
}

void EditObjectScreen::addEditorField(int& y, std::string name, float value, Panel & panel)
{
	std::ostringstream ss;
	parameterNamesLabels.push_back(new Label({ 30, y }, name, 20));
	ss << value;
	parameterValuesTextBoxes.push_back(new TextBox({ -120, y }, ss.str(), 20, 100, 8));
	panel.addChild(*parameterNamesLabels[parameterNamesLabels.size() - 1]);
	panel.addChild(*parameterValuesTextBoxes[parameterNamesLabels.size() - 1]);
	y -= 30;
}

void EditObjectScreen::loadNormalMapFromClipboard(const Input & input)
{
	std::string clipboardString = input.getClipboardString();
	
	normalMapFromClipboard = sceneImageStorage.addImage(clipboardString.c_str());
	pressNToLoadNormal.setText(normalMapFromClipboard.isValid()
		? std::string("normal map loaded, press enter")
		: std::string("map failed to load"));
	if (normalMapFromClipboard.isValid())
	{
		normalMapFromClipboard.getImage().transform(normalTransformation);
	}
}

void EditObjectScreen::loadTextureFromClipboard(const Input& input)
{
	std::string clipboardString = input.getClipboardString();

	textureFromClipboard = sceneImageStorage.addImage(clipboardString.c_str());
	pressLToLoadTexture.setText(textureFromClipboard.isValid()
		? std::string("texture loaded, press enter")
		: std::string("map failed to load"));
	
}
