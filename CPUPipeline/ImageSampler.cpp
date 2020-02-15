#include "ImageSampler.h"

ImageSampler::ImageSampler(ImageView image) :image(image)
{
	yMul = image.getImage().getImageHeight()-1;
	xMul = image.getImage().getImageWidth() - 1;
}




glm::vec3 ImageSampler::sample(glm::vec2 uv) const
{
	uv = glm::fract(uv);
	int index = (int)(glm::clamp(uv.x, 0.0f, 1.0f) * xMul)+
		 (int)(glm::clamp(uv.y, 0.0f, 1.0f) * yMul) * image.getImage().getImageWidth();
	return image.getImage().getData()[index];
}

Sampler* ImageSampler::getCopy() const
{
	return new ImageSampler(*this);
}

void ImageSampler::load(SceneDataReader& reader)
{
	image.load(reader);
	yMul = image.getImage().getImageHeight() - 1;
	xMul = image.getImage().getImageWidth() - 1;
}

void ImageSampler::save(SceneDataWriter& writer) const
{
	Sampler::save(writer);
	image.save(writer);
}
