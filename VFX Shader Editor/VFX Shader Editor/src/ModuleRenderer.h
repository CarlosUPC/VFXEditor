#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "GL/glew.h"

#define MAX_LIGHTS 8

class Shader;

class ModuleRenderer : public Module
{
public:

	ModuleRenderer(bool start_enabled = true);
	~ModuleRenderer();

	bool Init();
	update_state PreUpdate(float dt);
	update_state PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void GenerateFrameBuffer(int width, int height);

	void DrawGrid();
public:

	GLuint frameBuffer = 0;
	GLuint texture = 0;

	SDL_GLContext context;

	Shader* defaultShader = nullptr;

	uint VAO = 0;
};

#endif // !__MODULERENDER_H__