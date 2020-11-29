#pragma once

#include "Globals.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"


class Panel
{

public:

	Panel(const char* name)
		:name(name)
	{
		flags = ImGuiWindowFlags_None;
	}
	~Panel() {}

	virtual void Draw() {}

	const char* GetName() { return name.c_str(); }
	float2 GetSize() { return size; }
	bool IsActive() { return active; }
	void ToggleActive() { active = !active; }


protected:
	std::string name;
	ImGuiWindowFlags flags;

	float2 position;
	float2 size;

	bool active = true;
};


