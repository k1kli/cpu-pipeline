#include "Image.h"
#include "stb_image.h"

Image::Image(const char * filename)
{
	int channels;
	unsigned char * dataC = stbi_load(filename, &x, &y, &channels, 3);
	data = new glm::vec3[x * y];
	for (int i = 0; i < x * y; i++)
	{
		data[i].r = dataC[i * 3] / 255.0f;
		data[i].g = dataC[i * 3 + 1] / 255.0f;
		data[i].b = dataC[i * 3 + 2] / 255.0f;
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
