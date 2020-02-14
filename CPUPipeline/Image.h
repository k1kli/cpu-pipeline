#pragma once
#include <string>
#include <glm/glm.hpp>
#include <functional>
#include "ImageStorage.h"
class Image
{
	friend class ImageStorage;
	glm::vec3 * data;
	int x;
	int y;
	Image(const char* filename);
public:
	~Image();
	const glm::vec3* getData() const;
	int getImageWidth() const;
	int getImageHeight() const;
	void transform(std::function<glm::vec3(const glm::vec3&)> transformation);
};
glm::vec3 normalTransformation(const glm::vec3& input);

