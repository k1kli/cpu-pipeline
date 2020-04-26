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
	ImageSampler(SceneDataReader& reader) { load(reader); }
	virtual glm::vec3 sample(glm::vec2 uv) const;
	virtual Sampler* getCopy() const;
	SamplerType getType() const { return SamplerType::IMAGE; }
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;

};

