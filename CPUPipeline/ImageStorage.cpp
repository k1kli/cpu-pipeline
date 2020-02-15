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

void ImageStorage::load(SceneDataReader& reader)
{
	cleanup();
	int imageCount = reader.readInt();
	for (int i = 0; i < imageCount; i++)
	{
		int key = reader.readInt();
		Image* value = new Image(reader);
		images[key] = value;
		imageUseCounts[key] = 0;
	}
	reader.setImageStorage(*this);
	nextId = reader.readInt();
}

void ImageStorage::save(SceneDataWriter& writer) const
{
	int imageCount = images.size();
	writer.write(imageCount);
	for (auto imagePair : images)
	{
		writer.write(imagePair.first);
		imagePair.second->save(writer);
	}
	writer.write(nextId);
}

ImageStorage::~ImageStorage()
{
	cleanup();
}

void ImageStorage::cleanup()
{
	for (auto imagePair : images)
	{
		delete imagePair.second;
	}
	images.clear();
	imageUseCounts.clear();
}
