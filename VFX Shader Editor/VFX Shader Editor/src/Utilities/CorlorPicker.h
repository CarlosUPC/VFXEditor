#pragma once

#include "Globals.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\imgui_internal.h"

class ColorPicker
{

public:
	ColorPicker() {};
	~ColorPicker() {};

	void DisplayColorPicker(ImVec2 pos, float3& value );
	void DisplayInspector(bool open, float3& value);
	void OpenPicker(bool open, float3& value);

private:
	ImGuiColorEditFlags m_edit_flags;
	ImVec4 m_backup_color;
	ImVec4 m_color;
	float3 m_ret_color;
	std::string	m_popup_name;
	
	/*std::string	m_current_name;
	std::string	m_previous_name;*/
	
	
};