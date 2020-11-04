#include "Application.h"

// Include all Modules
#include "ModuleWindow.h"

Application::Application()
{
	window = new ModuleWindow();
	AddModule(window);
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
