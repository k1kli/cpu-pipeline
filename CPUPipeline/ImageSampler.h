#pragma once
#include "Image.h"
#include "Sampler.h"
#include "ImageView.h"
class ImageSampler : public Sampler
{
	ImageView image;
	int xMul;
	int yMul;
public:
	ImageSampler(ImageView image);
	virtual glm::vec3 sample(glm::vec2 uv) const;

};

