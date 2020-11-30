#include "PanelNodeGraph.h"

PanelNodeGraph::PanelNodeGraph(const char* name)
	:Panel(name)
{
}

PanelNodeGraph::~PanelNodeGraph()
{
}

void PanelNodeGraph::Draw()
{
	ImGui::Begin(name.c_str(), &active, flags);
	ImGui::Text("Hello Node Graph");
	ImGui::End();
}
