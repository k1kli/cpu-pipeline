#include "Sampler.h"
#include "ImageSampler.h"
#include "StaticColorSampler.h"

Sampler* Sampler::loadStatic(SceneDataReader& reader)
{
	SamplerType type = (SamplerType)reader.readInt();
	Sampler* res;
	switch (type)
	{
	case SamplerType::IMAGE:
		return new ImageSampler(reader);
	case SamplerType::STATIC_COLOR:
		return new StaticColorSampler(reader);
	}
}



void Sampler::save(SceneDataWriter& writer) const
{
	writer.write((int)getType());
}
