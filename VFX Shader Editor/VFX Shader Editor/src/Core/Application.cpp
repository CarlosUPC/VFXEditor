#include "Application.h"

// Include all Modules
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleCamera.h"
#include "ModuleResources.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	gui = new ModuleGUI();
	camera = new ModuleCamera();
	renderer = new ModuleRenderer();
	resources = new ModuleResources();

	AddModule(window);
	AddModule(input);
	AddModule(camera);
	AddModule(resources);

	AddModule(gui);
	AddModule(renderer);

	timer.Start();
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator module = modules.rbegin(); module != modules.rend() && (*module) != NULL; ++module)
	{
		RELEASE(*module);
	}
}

bool Application::Init()
{
	bool ret = true;
	
	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == true; ++module)
	{
		ret = (*module)->Init();
	}

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == true; ++module)
	{
		ret = (*module)->Start();
	}

	Random::StartRNGSeed();

	return ret;
}

void Application::CalcFrameTime()
{
	dt = (float)timer.ReadTime() / 1000.0f;
	
	std::cout << "Delta Time: " << dt << std::endl;
	timer.Start();
}

update_state Application::Update()
{
	update_state ret = update_state::UPDATE_CONTINUE;
	CalcFrameTime();

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == UPDATE_CONTINUE; ++module)
	{
		ret = (*module)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == UPDATE_CONTINUE; ++module)
	{
		ret = (*module)->Update(dt);
	}

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == UPDATE_CONTINUE; ++module)
	{
		ret = (*module)->PostUpdate(dt);
	}
	

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == true; ++module)
	{
		ret = (*module)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* module)
{
	modules.push_back(module);
}
