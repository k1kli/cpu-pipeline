#pragma once
#include "Image.h"
#include "ImageStorage.h"
class ImageStorage;
class Image;
class ImageView
{
public:
	ImageView(Image * image, ImageStorage * storage, int imageId, bool valid);
	ImageView();
	ImageView(const ImageView& other);
	~ImageView();
	ImageView& operator=(const ImageView& other);
	const Image& getImage() const;
	Image& getImage();
	const bool isValid() const;
private:
	int imageId;
	Image * image;
	ImageStorage * storage;
	bool valid;
};

