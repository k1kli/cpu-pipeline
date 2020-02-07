#include "ImageSampler.h"

ImageSampler::ImageSampler(const Image & image) :image(image)
{
	yMul = image.getImageHeight()-1;
	xMul = image.getImageWidth() - 1;
}

glm::vec3 ImageSampler::sample(glm::vec2 uv) const
{
	uv = glm::fract(uv);
	int index = (int)(glm::clamp(uv.x, 0.0f, 1.0f) * xMul)+
		 (int)(glm::clamp(uv.y, 0.0f, 1.0f) * yMul) * image.getImageWidth();
	return image.getData()[index];
}
