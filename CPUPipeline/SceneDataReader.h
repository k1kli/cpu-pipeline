#pragma once
#include <fstream>
#include <glm/glm.hpp>
#include "ImageStorage.h"
class SceneDataReader
{
public:
	SceneDataReader(std::ifstream& binaryStream) :inputStream(binaryStream) {}
	int readInt();
	float readFloat();
	glm::vec3 readVec3();
	glm::mat4 readMat4();
	ImageStorage& getImageStorage();
	void setImageStorage(ImageStorage & imageStorage);
private:
	std::ifstream& inputStream;
	ImageStorage* imageStorage = nullptr;

};

