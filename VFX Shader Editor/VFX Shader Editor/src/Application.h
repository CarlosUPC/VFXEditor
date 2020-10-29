#pragma once
#include "Globals.h"
#include <list>
#include "Module.h"

class Module;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_state Update();
	bool CleanUp();
private:
	void AddModule(Module* module);
private:

	std::list<Module*> modules;
	
};

extern Application* App;