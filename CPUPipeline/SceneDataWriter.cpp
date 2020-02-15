#include "SceneDataWriter.h"

void SceneDataWriter::write(float value)
{
	outputStream.write((const char *)&value, sizeof(float));
}

void SceneDataWriter::write(glm::vec3 value)
{
	write(value.x);
	write(value.y);
	write(value.z);
}

void SceneDataWriter::write(int value)
{
	outputStream.write((const char*)&value, sizeof(int));
}

void SceneDataWriter::write(glm::mat4 value)
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			write(value[y][x]);
		}
	}
}
