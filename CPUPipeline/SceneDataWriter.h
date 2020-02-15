#pragma once
#include <fstream>
#include <glm/glm.hpp>
class SceneDataWriter
{
public:
	SceneDataWriter(std::ofstream& binaryStream) :outputStream(binaryStream) {}
	void write(float value);
	void write(glm::vec3 value);
	void write(int value);
	void write(glm::mat4 value);
	void write(const char* bytes, int bytesSize);
private:
	std::ofstream & outputStream;
	
	
};

