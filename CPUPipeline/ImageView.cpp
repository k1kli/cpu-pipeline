#include "ImageView.h"

ImageView::ImageView(const Image& image, ImageStorage& storage, int imageId)
	:image(image),imageId(imageId), storage(storage)
{
}

ImageView::ImageView(const ImageView& other)
	:image(other.image), imageId(other.imageId), storage(other.storage)
{
	storage.reaquireImage(imageId);
}

ImageView::~ImageView()
{
	storage.releaseImage(imageId);
}

const Image& ImageView::getImage() const
{
	return image;
}

