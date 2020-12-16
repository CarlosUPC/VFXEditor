#include "ModuleResources.h"
#include "Resource.h"
#include "ResourceShader.h"
#include "Random.h"
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

ResourceShader* ModuleResources::CreateShader()
{
	ResourceShader* new_shader = new ResourceShader();
	new_shader->name = "Shader Graph";
	new_shader->id = Random::GenerateUUID();

	AddResource(new_shader);

	return new_shader;
	//new_shader->LoadShaderResource(vert,frag);
}
