#include "PanelInspector.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "PanelShaderEditor.h"
#include "ShaderGraph.h"
#include "ShaderNode.h"
PanelInspector::PanelInspector(const char* name)
	:Panel(name)
{
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 55, 200));
	ImGui::Begin(name.c_str(), 0);

	PanelShaderEditor* editor = (PanelShaderEditor*)App->gui->panels[SHADERGRAPH];
	ShaderGraph* graph = editor->GetGraph();

	if (graph != nullptr)
	{
		if (graph->node_selected != nullptr)
		{
			ImGui::Text(graph->node_selected->name.c_str());
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();
}
