#pragma once
#include "Module.h"
#include "GL/glew.h"

class Shader;

class ModuleRenderer : public Module
{
public:
	ModuleRenderer(bool start_enabled = true);
	~ModuleRenderer();

	bool Init();
	update_state PreUpdate();
	update_state PostUpdate();
	bool CleanUp();
public:
	//GLuint framebuffer = 0;
	SDL_GLContext context;

	Shader* default_shader = nullptr;
};
