#pragma once
#include "Image.h"
#include "ImageStorage.h"
class ImageStorage;
class Image;
class ImageView
{
public:
	ImageView(const Image& image, ImageStorage& storage, int imageId);
	ImageView(const ImageView& other);
	~ImageView();
	ImageView& operator=(const ImageView& other) = delete;
	const Image& getImage() const;
private:
	int imageId;
	const Image& image;
	ImageStorage& storage;
};

