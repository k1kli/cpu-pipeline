#pragma once
#include "Image.h"
#include "ImageView.h"
#include <map>
class ImageView;
class Image;
class ImageStorage
{
public:
	int addImage(Image* image);
	ImageView acquireImage(int imageId);
private:
	int nextId = 0;
	std::map<int, Image*> images;
	std::map<int, int> imageUseCounts;
	void releaseImage(int imageId);
	void reaquireImage(int imageId);
	friend class ImageView;
};

