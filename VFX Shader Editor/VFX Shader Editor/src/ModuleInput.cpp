#include "ModuleInput.h"

ModuleInput::ModuleInput(bool start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
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
	SDL_PumpEvents();
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	// KEYBOARD
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	//MOUSE
	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouseDeltaX = mouseDeltaY = mouseWheel = 0;

	bool quit = false;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEWHEEL:
			mouseWheel = event.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouseX = event.motion.x;
			mouseY = event.motion.y;

			mouseDeltaX = event.motion.xrel;
			mouseDeltaY = event.motion.yrel;
			break;
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


KEY_STATE ModuleInput::GetKey(int id) const
{
	return keyboard[id];
}

KEY_STATE ModuleInput::GetMouseButton(int id) const
{
	return mouse_buttons[id];
}

int ModuleInput::GetMouseX() const
{
	return mouseX;
}

int ModuleInput::GetMouseY() const
{
	return mouseY;
}

int ModuleInput::GetMouseWheel() const
{
	return mouseWheel;
}

int ModuleInput::GetMouseDeltaX() const
{
	return mouseDeltaX;
}

int ModuleInput::GetMouseDeltaY() const
{
	return mouseDeltaY;
}