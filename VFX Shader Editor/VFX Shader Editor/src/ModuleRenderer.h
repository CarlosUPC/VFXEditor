#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"

#define MAX_LIGHTS 8

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

	//void OnResize(int width, int height);

public:

	unsigned int frameBuffer = 0;

	SDL_GLContext context;

	Shader* defaultShader = nullptr;

	uint VAO = 0;
};

#endif // !__MODULERENDER_H__