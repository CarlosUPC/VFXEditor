#include "ModuleResources.h"

ModuleResources::ModuleResources(bool start_enabled)
	:Module(start_enabled)
{

}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{
	return true;
}

bool ModuleResources::Start()
{
	return true;
}

update_state ModuleResources::PreUpdate(float dt)
{
	return update_state::UPDATE_CONTINUE;
}

update_state ModuleResources::Update(float dt)
{
	return update_state::UPDATE_CONTINUE;
}

update_state ModuleResources::PostUpdate(float dt)
{
	return update_state::UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return false;
}

void ModuleResources::AddResource(Resource* res)
{
	if (res != nullptr)
	{
		if (std::find(resources.begin(), resources.end(), res) == resources.end())
		{
			resources.push_back(res);
		}
			
	}
}
