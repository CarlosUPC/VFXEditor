#pragma once

#include "Module.h"
#include "Panel.h"

class ModuleGUI : public Module
{

public:

	ModuleGUI(bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	bool Start();
	update_state PreUpdate(float dt);
	update_state Update(float dt);
	update_state PostUpdate(float dt);
	bool CleanUp();

	void Draw();

	bool UsingMouse() const;
	bool UsingKeyboard() const;

public:
	std::list<Panel*> panels;
	bool mouseOnScene = false;
};