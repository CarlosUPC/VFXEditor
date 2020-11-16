#pragma once
#include "Module.h"


class ModuleWindow : public Module
{
public:
	ModuleWindow(bool start_enabled = true);
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

public:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

	uint height = 0;
	uint width = 0;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;
};