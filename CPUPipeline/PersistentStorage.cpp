#include "PersistentStorage.h"
#include <sstream>
#include <fstream>
#include <iostream>

PersistentStorage::PersistentStorage(int saveSlot)
{
	std::ostringstream ss;
	ss << "data/save" << saveSlot;
	savePath = ss.str();
}

void PersistentStorage::save(SaveableObject& saveableObject)
{
	std::ofstream baseStream;
	baseStream.open(savePath, std::ios::out | std::ios::binary | std::ios::trunc);
	SceneDataWriter writer(baseStream);
	saveableObject.save(writer);
	baseStream.close();

}

void PersistentStorage::load(SaveableObject& saveableObject)
{
	std::ifstream baseStream;
	baseStream.open(savePath, std::ios::in | std::ios::binary);
	if (!baseStream)
	{
		throw "save not present";
	}
	SceneDataReader reader(baseStream);
	saveableObject.load(reader);
	baseStream.close();
}
