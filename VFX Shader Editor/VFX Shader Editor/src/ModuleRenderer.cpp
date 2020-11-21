#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include <iostream>
#include "ModuleCamera.h"
#include "Primitive.h"

//#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
//#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer::ModuleRenderer(bool start_enabled)
	:Module(start_enabled)
{
}

ModuleRenderer::~ModuleRenderer()
{
}

bool ModuleRenderer::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing glew! Error: %s\n", gluErrorString(error));
		ret = false;
	}

	if (ret)
	{
		std::cout << "Using Glew: " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		
		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		glMatrixMode(GL_PROJECTION);//Applies subsequent matrix operations to the projection matrix stack. (screen position)
		glLoadIdentity();
		glLoadMatrixf(App->camera->getProjectionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(App->camera->getViewMatrix());

		//specify implementation of The most correct, or highest quality, option should be chosen.
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		//Color and Depth buffers
		glClearDepth(1.0f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

	
	}


	return ret;
}

update_state ModuleRenderer::PreUpdate()
{
	//Render lights

	return UPDATE_CONTINUE;
}

update_state ModuleRenderer::PostUpdate()
{
	//Draw Scene
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(App->camera->getViewMatrix());

	MPlane base_plane(0, 1, 0, 0);
	base_plane.axis = true;
	base_plane.Render();

	//App->gui->Draw();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleRenderer::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}
