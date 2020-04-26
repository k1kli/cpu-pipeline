#pragma once
#include <glm/glm.hpp>
#include "SamplerType.h"
#include "SaveableObject.h"
class Sampler : SaveableObject
{
public:
	virtual glm::vec3 sample(glm::vec2 uv) const = 0;
	virtual Sampler* getCopy() const = 0;
	virtual SamplerType getType() const = 0;
	static Sampler* loadStatic(SceneDataReader& reader);
	virtual void load(SceneDataReader& reader) = 0;
	virtual void save(SceneDataWriter& writer) const;
	virtual ~Sampler() {}
};