#include "CorlorPicker.h"





void ColorPicker::DisplayColorPicker(ImVec2 pos, float3& value)
{
	m_edit_flags |= ImGuiColorEditFlags_AlphaBar;
	m_edit_flags |= ImGuiColorEditFlags_NoSidePreview;
	m_edit_flags |= ImGuiColorEditFlags_PickerHueBar;
	m_edit_flags |= ImGuiColorEditFlags_Float;

	ImGui::SetCursorScreenPos(ImVec2(pos));
	ImGui::ColorButton("##colorbutton", ImVec4(value.x, value.y, value.z, 1.0f), m_edit_flags, ImVec2(120, 25));
	
}

void ColorPicker::DisplayInspector(bool open, float3& value)
{

	m_edit_flags |= ImGuiColorEditFlags_AlphaBar;
	m_edit_flags |= ImGuiColorEditFlags_NoSidePreview;
	m_edit_flags |= ImGuiColorEditFlags_PickerHueBar;
	m_edit_flags |= ImGuiColorEditFlags_Float;

	ImGui::Text("Color ");
	ImGui::SameLine();
	if (ImGui::ColorButton("##colorbutton", ImVec4(value.x, value.y, value.z, 1.0f), m_edit_flags, ImVec2(300, 30)))
	{
		open = true;
	}
	std::string	m_button_name;

	OpenPicker(open, value);
	value = m_ret_color;
}

void ColorPicker::OpenPicker(bool open, float3& color)
{
	if (open)
	{
		ImGui::OpenPopup(m_popup_name.c_str());
		m_backup_color.x = color.x;
		m_backup_color.y = color.y;
		m_backup_color.z = color.z;
		m_backup_color.w = 1.0;
	}
	if (ImGui::BeginPopup(m_popup_name.c_str()))
	{
		ImGui::Text("ColorPicker");
		ImGui::Separator();
		ImGui::ColorPicker4("##pickercolor", (float*)&m_color, m_edit_flags);
		ImGui::SameLine();

		/*ImGui::BeginGroup();
		ImGui::Text("Current");
		ImGui::ColorButton(m_current_name.c_str(), m_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous");
		if (ImGui::ColorButton(m_previous_name.c_str(), m_backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			m_color = m_backup_color;
		ImGui::EndGroup();*/
		ImGui::EndPopup();
	}

	m_ret_color.x = m_color.x;
	m_ret_color.y = m_color.y;
	m_ret_color.z = m_color.z;



}
