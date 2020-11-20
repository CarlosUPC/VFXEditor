#include "Application.h"
#include "ModuleGUI.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"


#include "ModuleWindow.h"
#include "ModuleRenderer.h"

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
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = "../Settings/imgui.ini";

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL2_Init();

	

	return true;
}

bool ModuleGUI::Start()
{
	//Set Default Style

	//Load GUI textures

	return true;
}

update_state ModuleGUI::PreUpdate()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//Draw Central Docking Window
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ (float)App->window->width, (float)App->window->height });
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", 0, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

	ImGui::End();
	return UPDATE_CONTINUE;
}

update_state ModuleGUI::Update()
{
	return UPDATE_CONTINUE;
}

update_state ModuleGUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}


bool ModuleGUI::CleanUp()
{
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGUI::Draw()
{

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}