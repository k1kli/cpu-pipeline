#include "ImageStorage.h"

int ImageStorage::addImage(Image* image)
{
	images[nextId] = image;
	imageUseCounts[nextId] = 0;
	return nextId++;
}

ImageView ImageStorage::acquireImage(int imageId)
{
	Image* image = images.at(imageId);
	imageUseCounts[imageId]++;
	return ImageView(*image, *this, imageId);
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
