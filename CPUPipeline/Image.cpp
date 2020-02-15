#include "Image.h"
#include "stb_image.h"

Image::Image(const char * filename)
{
	int channels;
	unsigned char * dataC = stbi_load(filename, &width, &height, &channels, 3);
	if (dataC == nullptr)
	{
		throw "invalid file";
	}
	data = new glm::vec3[width * height];
	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			data[yi * width + xi].r = dataC[((height - yi - 1) * width + xi) * 3] / 255.0f;
			data[yi * width + xi].g = dataC[((height - yi - 1) * width + xi) * 3 + 1] / 255.0f;
			data[yi * width + xi].b = dataC[((height - yi - 1) * width + xi) * 3 + 2] / 255.0f;
		}
	}
	stbi_image_free(dataC);
}

Image::~Image()
{
	if (data != nullptr) delete[] data;
}

const glm::vec3* Image::getData() const
{
	return data;
}
int Image::getImageWidth() const
{
	return width;
}

int Image::getImageHeight() const
{
	return height;
}

void Image::transform(std::function<glm::vec3(const glm::vec3&)> transformation)
{
	for (int i = 0; i < width * height; i++)
	{
		data[i] = transformation(data[i]);
	}
}

void Image::load(SceneDataReader& reader)
{
	if (data != nullptr) delete[] data;
	width = reader.readInt();
	height = reader.readInt();
	data = new glm::vec3[width * height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			data[y * width + x] = reader.readVec3();
		}
	}
}

void Image::save(SceneDataWriter& writer) const
{
	writer.write(width);
	writer.write(height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			writer.write(data[y * width + x]);
		}
	}
}

glm::vec3 normalTransformation(const glm::vec3& input)
{
	return glm::normalize(glm::vec3(
		(input.x * 2.0f - 1.0f),
		(input.y * 2.0f - 1.0f),
		input.z * 2.0f - 1.0f));
}
