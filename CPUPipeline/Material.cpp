#include "Material.h"

Material::~Material()
{
	delete colorSampler;
	delete normalSampler;
}

Material::Material(const Material& other)
{
	specular = other.specular;
	ambient = other.ambient;
	shininess = other.shininess;
	if (other.colorSampler != nullptr)
		colorSampler = other.colorSampler->getCopy();
	else
		colorSampler = nullptr;
	if (other.normalSampler != nullptr)
		normalSampler = other.normalSampler->getCopy();
	else
		normalSampler = nullptr;
}

Material& Material::operator=(const Material& other)
{
	if (&other == this) return *this;
	if (colorSampler != nullptr)
	{
		delete colorSampler;
	}
	if (normalSampler != nullptr)
	{
		delete normalSampler;
	}

	specular = other.specular;
	ambient = other.ambient;
	shininess = other.shininess;
	if (other.colorSampler != nullptr)
		colorSampler = other.colorSampler->getCopy();
	else
		colorSampler = nullptr;
	if (other.normalSampler != nullptr)
		normalSampler = other.normalSampler->getCopy();
	else
		normalSampler = nullptr;
	return *this;
}

const Sampler& Material::getColorSampler() const
{
	return *colorSampler;
}

const Sampler& Material::getNormalSampler() const
{
	return *normalSampler;
}

void Material::setColorSampler(const Sampler & newSampler)
{
	if (colorSampler != nullptr)
		delete colorSampler;
	colorSampler = newSampler.getCopy();
}

void Material::setNormalSampler(const Sampler& newSampler)
{
	if (normalSampler != nullptr)
		delete normalSampler;
	normalSampler = newSampler.getCopy();
}

void Material::load(SceneDataReader& reader)
{
	specular = reader.readVec3();
	ambient = reader.readVec3();
	shininess = reader.readFloat();
	colorSampler = Sampler::loadStatic(reader);
	normalSampler = Sampler::loadStatic(reader);
}

void Material::save(SceneDataWriter& writer) const
{
	writer.write(specular);
	writer.write(ambient);
	writer.write(shininess);
	colorSampler->save(writer);
	normalSampler->save(writer);
}
