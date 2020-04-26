#pragma once
#include "SceneDataReader.h"
#include "SceneDataWriter.h"
class SaveableObject
{
public:
	virtual void load(SceneDataReader & reader) = 0;
	virtual void save(SceneDataWriter & writer) const = 0;
};

