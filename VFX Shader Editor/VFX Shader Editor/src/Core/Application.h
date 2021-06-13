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
class ModuleResources;

#include "Texture.h"

class Application
{
//METHODS
public:
	Application();
	~Application();

	bool Init();
	float GetDT();
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
	ModuleResources*		resources = nullptr;

	//temp
	std::vector<Texture>  textures;
private:
	std::list<Module*> modules;
	Timer timer;
	float dt;
};

extern Application* App;