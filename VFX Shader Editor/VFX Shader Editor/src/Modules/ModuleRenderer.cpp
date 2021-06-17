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

//#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
//#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
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
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			//ret = false;
		}

		
		
		renderQuadTangentSpace();
		//Test
		{
			//float vertices[] = {
			////positions			 //text coords
			// 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
			// 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
			//-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
			//-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
			//};
			//unsigned int indices[] = {  
			//	0, 1, 3,  
			//	1, 2, 3   
			//};
			//



			//unsigned int VBO, EBO;
			//glGenVertexArrays(1, &VAO);
			//glGenBuffers(1, &VBO);
			//glGenBuffers(1, &EBO);
			//
			//glBindVertexArray(VAO);

			//glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			////position attribute
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			//glEnableVertexAttribArray(0);
			////texture coord attribute
			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			//glEnableVertexAttribArray(1);

			//
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//glBindVertexArray(0);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

		//Default data
		float3 lightPos = float3(0.0f, 0.0f, 1.0f);
		
		//Matrix data
		current_shader->SetUniformMat4f("u_View", App->camera->getViewMatrix());
		current_shader->SetUniformMat4f("u_Projection", App->camera->getProjectionMatrix());
		float4x4 modelMatrix = float4x4().identity;
		current_shader->SetUniformMat4f("u_Model", (GLfloat*)&modelMatrix);
		
		current_shader->SetUniform3f("lightPos", (float)lightPos.x, (float)lightPos.y, (float)lightPos.z);
		current_shader->SetUniform3f("viewPos", App->camera->GetCameraPosition().x, App->camera->GetCameraPosition().y, App->camera->GetCameraPosition().z);
		
	
		//Update dinamic Uniforms
		for (auto it = current_shader->graph->uniforms.begin(); it != current_shader->graph->uniforms.end(); ++it)
		{
			it->second->Bind(current_shader);
		}


		//Draw data
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (current_shader != nullptr)
		current_shader->Unbind();

	glBindTexture(GL_TEXTURE_2D, 0);
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

	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

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

void ModuleRenderer::renderQuadTangentSpace()
{

	if (VAO == 0)
	{
		// positions
		float3 pos1(-1.0f, 1.0f, 0.0f);
		float3 pos2(-1.0f, -1.0f, 0.0f);
		float3 pos3(1.0f, -1.0f, 0.0f);
		float3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		float2 uv1(0.0f, 1.0f);
		float2 uv2(0.0f, 0.0f);
		float2 uv3(1.0f, 0.0f);
		float2 uv4(1.0f, 1.0f);
		// normal vector
		float3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		float3 tangent1, bitangent1;
		float3 tangent2, bitangent2;
		// triangle 1
		// ----------
		float3 edge1 = pos2 - pos1;
		float3 edge2 = pos3 - pos1;
		float2 deltaUV1 = uv2 - uv1;
		float2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = tangent1.Normalized();
		
		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = bitangent1.Normalized();

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = tangent2.Normalized();


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = bitangent2.Normalized();


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};

		unsigned int VBO;
		// configure plane VAO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);

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
