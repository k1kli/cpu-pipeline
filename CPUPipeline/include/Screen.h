#pragma once
#include "Panel.h"
#include "Input.h"
class Screen : public Panel
{
public:
	Screen() :Panel(0, { 0,0 }, -1, -1) {}
	virtual void handleInput(const Input & input) = 0;
	virtual ~Screen() {}

};

