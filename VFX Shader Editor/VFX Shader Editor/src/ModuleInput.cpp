#include "ModuleInput.h"

ModuleInput::ModuleInput(bool start_enabled)
{
}

ModuleInput::~ModuleInput()
{
}

bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	
	SDL_Init(0);
	
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	
	return ret;
}

update_state ModuleInput::PreUpdate()
{
	bool quit = false;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
	}

	if (quit)
		return update_state::UPDATE_STOP;

	return update_state::UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}
