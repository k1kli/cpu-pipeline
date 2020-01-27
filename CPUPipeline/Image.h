#pragma once
#include <string>
#include <glm/glm.hpp>
class Image
{
	glm::vec3 * data;
	int x;
	int y;
public:
	Image(const char* filename);
	~Image();
	const glm::vec3* getData() const;
	int getImageWidth() const;
	int getImageHeight() const;
};

