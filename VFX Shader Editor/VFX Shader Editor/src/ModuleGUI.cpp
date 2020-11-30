#include "Application.h"
#include "ModuleGUI.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"


#include "ModuleWindow.h"
#include "ModuleRenderer.h"

#include "PanelPreview.h"
#include "PanelNodeGraph.h"

ModuleGUI::ModuleGUI(bool start_enabled)
	: Module(start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.IniFilename = "Config/imgui.ini";

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init();
	

	return true;
}

bool ModuleGUI::Start()
{
	//Set Default Style

	//Load GUI textures

	//Load panels
	panels.push_back(new PanelPreview("Preview"));
	panels.push_back(new PanelNodeGraph("Node Graph"));
	return true;
}

update_state ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//Draw Central Docking Window
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ (float)App->window->width, (float)App->window->height });
	//ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", 0, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));



	return UPDATE_CONTINUE;
}

update_state ModuleGUI::Update(float dt)
{
	
	for (std::list<Panel*>::iterator it_p = panels.begin(); it_p != panels.end(); ++it_p)
	{
		if ((*it_p)->IsActive())
		{
			(*it_p)->Draw();
		}
	}

	return UPDATE_CONTINUE;
}

update_state ModuleGUI::PostUpdate(float dt)
{
	ImGui::End();
	return UPDATE_CONTINUE;
}


bool ModuleGUI::CleanUp()
{
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	for (std::list<Panel*>::iterator it_p = panels.begin(); it_p != panels.end(); ++it_p)
	{
		RELEASE((*it_p));
	}
	panels.clear();

	return true;
}

void ModuleGUI::Draw()
{
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	/*ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(App->window->window, App->renderer->context);
	}*/
}