#pragma once
#include <string>
#include "SaveableObject.h"
//storage model based on https://catlikecoding.com/unity/tutorials/object-management/persisting-objects/
class PersistentStorage
{
public:
	PersistentStorage(int saveSlot);
	void save(SaveableObject& saveableObject);
	void load(SaveableObject& saveableObject);
	virtual ~PersistentStorage() {}
private:
	std::string savePath;
	
};

