#pragma once
#include "SaveableObject.h"
#include "Image.h"
#include "ImageView.h"
#include <map>
class ImageView;
class Image;
class ImageStorage : public SaveableObject
{
public:
	ImageView addImage(const char * filepath);
	ImageView acquireImage(int imageId);
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;
	~ImageStorage();
private:
	int nextId = 0;
	std::map<int, Image*> images;
	std::map<int, int> imageUseCounts;
	void releaseImage(int imageId);
	void reaquireImage(int imageId);
	void cleanup();
	friend class ImageView;
};

