#pragma once
#include "Image.h"
#include "Sampler.h"
class ImageSampler : public Sampler
{
	const Image & image;
	int xMul;
	int yMul;
public:
	ImageSampler(const Image & image);
	virtual glm::vec3 sample(glm::vec2 uv) const;

};

