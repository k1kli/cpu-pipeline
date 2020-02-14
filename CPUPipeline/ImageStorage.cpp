#include "ImageStorage.h"


ImageView ImageStorage::addImage(const char* filepath)
{
	Image* image;
	try
	{
		image = new Image(filepath);
	}
	catch(...)
	{
		return ImageView(nullptr, this, -1, false);
	}
	images[nextId] = image;
	imageUseCounts[nextId] = 1;
	nextId++;
	return ImageView(image, this, nextId-1, true);
}

ImageView ImageStorage::acquireImage(int imageId)
{
	Image* image = images.at(imageId);
	imageUseCounts[imageId]++;
	return ImageView(image, this, imageId, true);
}

void ImageStorage::releaseImage(int imageId)
{
	Image* image = images.at(imageId);
	imageUseCounts[imageId]--;
	if (imageUseCounts[imageId] == 0)
	{
		delete images[imageId];
		images.erase(imageId);
		imageUseCounts.erase(imageId);
	}
}

void ImageStorage::reaquireImage(int imageId)
{
	Image* image = images.at(imageId);
	imageUseCounts[imageId]++;
}
