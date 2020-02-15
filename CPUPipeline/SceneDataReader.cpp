#include "SceneDataReader.h"

int SceneDataReader::readInt()
{
	int res;
	inputStream.read((char *)&res, sizeof(int));
	return res;
}

float SceneDataReader::readFloat()
{
	float res;
	inputStream.read((char*)&res, sizeof(float));	
	return res;
}

glm::vec3 SceneDataReader::readVec3()
{
	glm::vec3 res;
	res.x = readFloat();
	res.y = readFloat();
	res.z = readFloat();
	return res;
}

glm::mat4 SceneDataReader::readMat4()
{
	glm::mat4 res;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			res[y][x] = readFloat();
		}
	}
	return res;
}

char* SceneDataReader::readBytesArray(int * sizeRes)
{
	*sizeRes = readInt();
	char* res = new char[*sizeRes];
	inputStream.read(res, *sizeRes);
	return res;
}

ImageStorage& SceneDataReader::getImageStorage()
{
	return *imageStorage;
}

void SceneDataReader::setImageStorage(ImageStorage& imageStorage)
{
	this->imageStorage = &imageStorage;
}
