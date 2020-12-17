#pragma once

#include "Panel.h"

class ResourceShader;

class PanelShaderEditor : public Panel
{
public:

	PanelShaderEditor(const char* name);
	~PanelShaderEditor();

	void Draw();
	void OnShaderAction(std::string& action);
	void CreateNewShaderPopUp();
	void LoadShaderPopUp();

private:
	bool grid = true;
	float2 scrollCoords = { 0.0f, 0.0f };


	ResourceShader* current_shader = nullptr;
	bool creating_shader = false;
	bool selecting_shader = false;
};
