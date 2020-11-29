#pragma once
#include "Globals.h"
#include <list>
#include "Module.h"
#include "Timer.h"
#include "Random.h"

class Module;
class ModuleWindow;
class ModuleRenderer;
class ModuleInput;
class ModuleGUI;
class ModuleCamera;

class Application
{
//METHODS
public:
	Application();
	~Application();

	bool Init();
	void CalcFrameTime();
	update_state Update();
	bool CleanUp();
private:
	void AddModule(Module* module);

//VARIABLES
public:
	ModuleWindow*			window = nullptr;
	ModuleRenderer*			renderer = nullptr;
	ModuleInput*			input = nullptr;
	ModuleGUI*				gui = nullptr;
	ModuleCamera*			camera = nullptr;
private:
	std::list<Module*> modules;
	Timer timer;
	float dt;
};

extern Application* App;