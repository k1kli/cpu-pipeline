#include "Image.h"
#include "stb_image.h"

Image::Image(const char * filename)
{
	int channels;
	unsigned char * dataC = stbi_load(filename, &x, &y, &channels, 3);
	data = new glm::vec3[x * y];
	for (int yi = 0; yi < y; yi++)
	{
		for (int xi = 0; xi < x; xi++)
		{
			data[yi * x + xi].r = dataC[((y - yi - 1) * x + xi) * 3] / 255.0f;
			data[yi * x + xi].g = dataC[((y - yi - 1) * x + xi) * 3 + 1] / 255.0f;
			data[yi * x + xi].b = dataC[((y - yi - 1) * x + xi) * 3 + 2] / 255.0f;
		}
	}
	stbi_image_free(dataC);
}

Image::~Image()
{
	delete[] data;
}

const glm::vec3* Image::getData() const
{
	return data;
}
int Image::getImageWidth() const
{
	return x;
}

int Image::getImageHeight() const
{
	return y;
}

void Image::transform(std::function<glm::vec3(const glm::vec3&)> transformation)
{
	for (int i = 0; i < x * y; i++)
	{
		data[i] = transformation(data[i]);
	}
}

glm::vec3 normalTransformation(const glm::vec3& input)
{
	return glm::normalize(glm::vec3(
		(input.x * 2.0f - 1.0f),
		(input.y * 2.0f - 1.0f),
		input.z * 2.0f - 1.0f));
}
