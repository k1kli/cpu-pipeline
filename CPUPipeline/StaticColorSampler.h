#pragma once
#include "Sampler.h"
class StaticColorSampler : public Sampler
{
	glm::vec3 color;
public:
	StaticColorSampler(const glm::vec3& color) :color(color) {}
	StaticColorSampler(SceneDataReader& reader) { load(reader); }
	virtual glm::vec3 sample(glm::vec2 uv) const;
	virtual Sampler* getCopy() const;
	const glm::vec3& getColor() const;
	SamplerType getType() const { return SamplerType::STATIC_COLOR; }
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;
};

