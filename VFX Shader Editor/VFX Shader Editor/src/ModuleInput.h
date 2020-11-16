#pragma once
#include "Module.h"
#include "SDL/include/SDL_scancode.h"

class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_state PreUpdate();
	bool CleanUp();
private:

};
