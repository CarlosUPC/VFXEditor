#pragma once
#include "Globals.h"
#include <list>
#include "Module.h"

class Module;
class ModuleWindow;

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
private:
	std::list<Module*> modules;
};

extern Application* App;