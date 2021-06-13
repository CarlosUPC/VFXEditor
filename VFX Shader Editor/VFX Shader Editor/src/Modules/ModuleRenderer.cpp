#include "Application.h"
#include "ModuleRenderer.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include "ModuleCamera.h"
#include "ResourceShader.h"
#include "ShaderGraph.h"
#include "ShaderUniform.h"

//TMP
#include "Primitive.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
//#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer::ModuleRenderer(bool start_enabled) : Module(start_enabled)
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

	if (ret == true)
	{
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));

		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(App->camera->getProjectionMatrix());

		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(App->camera->getViewMatrix());
		
		glClearDepth(1.0f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		
		

		//Test
		{
			float vertices[] = {
			//positions			 //text coords
			 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
			};
			unsigned int indices[] = {  
				0, 1, 3,  
				1, 2, 3   
			};
			



			unsigned int VBO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			//position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			//texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		//Create Shader program
		//current_shader = new ResourceShader("Shaders/default.vs", "Shaders/default.fs");
	}

	GenerateFrameBuffer(App->window->width, App->window->height);
	
	return ret;
}

update_state ModuleRenderer::PreUpdate(float dt)
{
	//Render lights
	
	return UPDATE_CONTINUE;
}

update_state ModuleRenderer::PostUpdate(float dt)
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glViewport(0, 0, viewport_w, viewport_h);				
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temporal (Debug)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->getProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->getViewMatrix());
	DrawGrid();
	

	if (current_shader != nullptr)
	{
		////Bind shader
		current_shader->Bind();

		////Send data to shader
		current_shader->SetUniformMat4f("u_Projection", App->camera->getProjectionMatrix());
		current_shader->SetUniformMat4f("u_View", App->camera->getViewMatrix());

		/*for (auto it = current_shader->graph->uniforms.begin(); it != current_shader->graph->uniforms.end(); ++it)
		{
			it->second->Bind(current_shader);
		}*/
	}

	//Draw data
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	if (current_shader != nullptr)
		current_shader->Unbind();

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	App->gui->Draw(); // Draw GUI

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleRenderer::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer::GenerateFrameBuffer(int width, int height)
{
	viewport_w = width; viewport_h = height;

	//Generate framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Generate texture buffer
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Attach it to currently framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[ERROR]: Framebuffer is not complete!");
	}

	//glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void ModuleRenderer::DrawGrid()
{
	glLineWidth(1.0f);

	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);

	float d = 5.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();



}

void ModuleRenderer::OnResize(int width, int height)
{

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->getProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(App->camera->getViewMatrix());*/

	// Delete previous Framebuffer & texture
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &texture);

	//Resize Framebuffer
	GenerateFrameBuffer(width, height);
	
	//glViewport(0, 0, width, height);
}
