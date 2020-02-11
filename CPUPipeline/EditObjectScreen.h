#pragma once
#include "Screen.h"
#include "SceneObject.h"
#include <functional>
#include "Label.h"
#include "TextBox.h"
class EditObjectScreen :
	public Screen
{
public:
	EditObjectScreen(std::function<void()> doneCallback, SceneObject& sceneObjectToModify);
	void handleInput(const Input& input);
	~EditObjectScreen();
private:
	std::function<void()> doneCallback;
	SceneObject& sceneObjectToModify;

	Panel sidePanel = Panel(RGBA(255, 255, 255, 100), { 0,0 }, 500, -1);
	Label editMeshLabel = Label({ 30, -40 }, std::string("Edit mesh"), 30);
	Label meshNameLabel = Label({ 30, -80 }, std::string("--meshname--"), 20);
	Label pressTabToJumpToNextField = Label({ 30, 60 }, std::string("press tab to jump to next field"), 15);
	Label pressEnterToApply = Label({ 30, 40 }, std::string("press ENTER to apply"), 15);
	Label pressVAgainToLeave = Label({ 30, 20 }, std::string("press V again to leave"), 15);
	std::vector<Label *> parameterNamesLabels;
	std::vector<TextBox *> parameterValuesTextBoxes;
	int selectedParameterId = 0;
	void selectNextParameter();
	void tryApply();
};
