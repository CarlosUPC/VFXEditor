#pragma once
#include "Module.h"
#include "SDL/include/SDL_scancode.h"


#define MAX_KEYS 300
#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};


class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_state PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const;
	KEY_STATE GetMouseButton(int id) const;

	int GetMouseX() const;
	int GetMouseY() const;

	int GetMouseWheel() const;
	int GetMouseDeltaX() const;
	int GetMouseDeltaY() const;

private:
	KEY_STATE* keyboard = nullptr;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];


	int mouseX;
	int mouseY;
	int mouseDeltaX;
	int mouseDeltaY;
	int mouseWheel;
};
