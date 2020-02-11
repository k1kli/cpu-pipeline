#pragma once
#include "Screen.h"
#include "SceneObject.h"
#include <functional>
#include "Label.h"
#include "TextBox.h"
#include "ImageStorage.h"
class EditObjectScreen :
	public Screen
{
public:
	EditObjectScreen(std::function<void()> doneCallback, SceneObject& sceneObjectToModify, ImageStorage & imageStorage);
	void handleInput(const Input& input);
	~EditObjectScreen();
private:
	std::function<void()> doneCallback;
	SceneObject& sceneObjectToModify;

	Panel sidePanel = Panel(RGBA(255, 255, 255, 100), { 0,0 }, 500, -1);
	Label editMeshLabel = Label({ 30, -40 }, std::string("Edit mesh"), 30);
	Label meshNameLabel = Label({ 30, -80 }, std::string("--meshname--"), 20);
	Label pressVAgainToLeave = Label({ 30, 20 }, std::string("press V again to leave"), 15);
	Label pressEnterToApply = Label({ 30, 40 }, std::string("press ENTER to apply"), 15);
	Label pressTabToJumpToNextField = Label({ 30, 60 }, std::string("press tab to jump to next field"), 15);
	Label pressNToLoadNormal = Label({ 30, 80 }, std::string("press N to load normal map from clipboard filename"), 15);
	Label pressLToLoadTexture = Label({ 30, 100 }, std::string("press L to load texture from clipboard filename"), 15);
	Label pressMToSwitchColorSampler = Label({ 30, 120 }, std::string("press M to switch to static object color"), 15);
	std::vector<Label *> parameterNamesLabels;
	std::vector<TextBox *> parameterValuesTextBoxes;
	ImageView normalMapFromClipboard;
	ImageView textureFromClipboard;
	ImageStorage& sceneImageStorage;
	int selectedParameterId = 0;
	int meshParametersEnd = 0;
	bool diffuseFromTexture = false;
	void selectNextParameter();
	void tryApply();
	void createMesh();
	void createMaterial();
	void loadEditor();
	void loadMeshEditor(int& y);
	void loadMaterialEditor(int &y);
	void addEditorField(int& y, std::string name, float value);
	void loadNormalMapFromClipboard(const Input& input);
	void loadTextureFromClipboard(const Input& input);
};
