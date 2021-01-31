#pragma once

#include "Module.h"
//#include <unordered_map>

class Resource;
class ResourceShader;

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

	ResourceShader* CreateShader(const std::string& m_Name);

private:
	std::vector<Resource*> resources;
};
