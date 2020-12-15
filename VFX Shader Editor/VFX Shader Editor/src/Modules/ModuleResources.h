#pragma once

#include "Module.h"
#include <unordered_map>
#include "Resource.h"
#include <vector>
class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init();
	bool Start();
	update_state PreUpdate(float dt);
	update_state Update(float dt);
	update_state PostUpdate(float dt);
	bool CleanUp();

	void AddResource(Resource* res);

private:
	std::vector<Resource*> resources;
};
