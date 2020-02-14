#pragma once
#include "Screen.h"
#include "Light.h"
#include <functional>
#include "Label.h"
#include "TextBox.h"
class EditLightScreen :
	public Screen
{
public:
	EditLightScreen(std::function<void()> doneCallback, Light& lightToModify);
	void handleInput(const Input& input);
	~EditLightScreen();
private:
	std::function<void()> doneCallback;
	Light& lightToModify;

	Panel leftSidePanel = Panel(RGBA(255, 255, 255, 100), { 0,0 }, 500, -1);
	Label pressVAgainToLeave = Label({ 30, 20 }, std::string("press V again to leave"), 15);
	Label pressEnterToApply = Label({ 30, 40 }, std::string("press ENTER to apply"), 15);
	Label pressTabToJumpToNextField = Label({ 30, 60 }, std::string("press tab to jump to next field (+shift to previous)"), 15);
	Label editLightLabel = Label({ 30, 60 }, std::string("Edit light"), 40);
	Label diffuseLabel = Label({ 30, 60 }, std::string("Diffuse color"), 30);
	Label specularLabel = Label({ 30, 60 }, std::string("Specular color"), 30);
	Label positionLabel = Label({ 30, 60 }, std::string("Position"), 30);
	Label attenuationLabel = Label({ 30, 60 }, std::string("Attenuation"), 30);
	std::vector<Label*> parameterNamesLabels;
	std::vector<TextBox*> parameterValuesTextBoxes;
	int selectedParameterId = 0;
	void selectNextParameter();
	void selectPreviousParameter();
	void tryApply();

	void addEditorField(int& y, std::string name, float value, Panel& panel);
	void loadEditor();
};

