#pragma once
#include "Globals.h"
#include <list>
#include "Module.h"

class Module;
class ModuleWindow;
class ModuleRenderer;
class ModuleInput;

class Application
{
//METHODS
public:
	Application();
	~Application();

	bool Init();
	update_state Update();
	bool CleanUp();
private:
	void AddModule(Module* module);

//VARIABLES
public:
	ModuleWindow*			window = nullptr;
	ModuleRenderer*			renderer = nullptr;
	ModuleInput*			input = nullptr;
private:
	std::list<Module*> modules;
};

extern Application* App;