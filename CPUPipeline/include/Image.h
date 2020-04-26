#pragma once
#include <string>
#include <glm/glm.hpp>
#include <functional>
#include "SaveableObject.h"
#include "ImageStorage.h"
class Image : public SaveableObject
{
	friend class ImageStorage;
public:
	~Image();
	const glm::vec3* getData() const;
	int getImageWidth() const;
	int getImageHeight() const;
	void transform(std::function<glm::vec3(const glm::vec3&)> transformation);
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;
private:
	glm::vec3* data = nullptr;
	int width;
	int height;
	Image(const char* filename);
	Image(SceneDataReader& reader) { load(reader); }
};
glm::vec3 normalTransformation(const glm::vec3& input);

