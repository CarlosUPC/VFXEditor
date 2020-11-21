#include "Application.h"

// Include all Modules
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleCamera.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	//gui = new ModuleGUI();
	camera = new ModuleCamera();
	renderer = new ModuleRenderer();

	AddModule(window);
	AddModule(input);
	//AddModule(gui);
	AddModule(camera);
	AddModule(renderer);
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

	return ret;
}

update_state Application::Update()
{
	update_state ret = update_state::UPDATE_CONTINUE;

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == UPDATE_CONTINUE; ++module)
	{
		ret = (*module)->PreUpdate();
	}

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == UPDATE_CONTINUE; ++module)
	{
		ret = (*module)->Update();
	}

	for (std::list<Module*>::iterator module = modules.begin(); module != modules.end() && ret == UPDATE_CONTINUE; ++module)
	{
		ret = (*module)->PostUpdate();
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
