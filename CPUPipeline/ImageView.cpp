#include "ImageView.h"

ImageView::ImageView(Image* image, ImageStorage * storage, int imageId, bool valid)
	:image(image),imageId(imageId), storage(storage), valid(valid)
{
}
ImageView::ImageView()
	: image(nullptr), imageId(0), storage(nullptr), valid(false)
{

}


ImageView::ImageView(const ImageView& other)
	:ImageView(other.image, other.storage, other.imageId,other.valid)
{
	storage->reaquireImage(imageId);
}

ImageView::~ImageView()
{
	if(valid)
		storage->releaseImage(imageId);
}

ImageView& ImageView::operator=(const ImageView& other)
{
	if (other.image == image) return *this;
	if (other.valid)
	{
		other.storage->reaquireImage(other.imageId);
	}
	if (valid)
	{
		storage->releaseImage(imageId);
	}
	image = other.image;
	storage = other.storage;
	imageId = other.imageId;
	valid = other.valid;
	return *this;
}

const Image& ImageView::getImage() const
{
	return *image;
}
Image& ImageView::getImage()
{
	return *image;
}

const bool ImageView::isValid() const
{
	return valid;
}

void ImageView::load(SceneDataReader& reader)
{
	if (valid)
	{
		storage->releaseImage(imageId);
	}
	int tempImageId = reader.readInt();
	bool tempValid = (bool)reader.readInt();
	if (tempValid)
	{
		storage = &reader.getImageStorage();
		*this = storage->acquireImage(tempImageId);
	}
}

void ImageView::save(SceneDataWriter& writer) const
{
	writer.write(imageId);
	writer.write((int)valid);
}

