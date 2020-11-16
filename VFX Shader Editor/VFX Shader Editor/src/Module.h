#pragma once

#include "Globals.h"
#include "SDL/include/SDL.h"

class Module
{
private:
	bool enabled;
public:
	Module(bool start_enabled = true)
		: enabled(start_enabled){}

	virtual ~Module() {}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_state PreUpdate()
	{
		return UPDATE_CONTINUE;
	}
	virtual update_state Update()
	{
		return UPDATE_CONTINUE;
	}
	virtual update_state PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}
};
