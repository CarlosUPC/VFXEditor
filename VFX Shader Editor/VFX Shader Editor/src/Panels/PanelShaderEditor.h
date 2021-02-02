#pragma once

#include "Panel.h"
#include <functional>

class ResourceShader;
class ShaderNode;
class ShaderGraph;
enum NodeType;


typedef ShaderNode* (ShaderGraph::* CreateNodeFn)(const char* n, int t);
typedef std::function<ShaderNode * (const char*, int)> create_node;

class PanelShaderEditor : public Panel
{
public:

	PanelShaderEditor(const char* name);
	~PanelShaderEditor();

	void Draw();
	void OnShaderAction(std::string& action);
	void CreateNewShaderPopUp();
	void LoadShaderPopUp();
	void AddNewNodePopUp();

	void AddNode(ShaderGraph& graph, ShaderNode* node);

	ShaderNode CreateNode();
	void NodeOption(const char* name, NodeType type, ResourceShader* shader, ShaderGraph* graph, ShaderNode* (ShaderGraph::*p)(const char* n, int t));
	void NodeOption(const char* name, NodeType type, ResourceShader* shader, ShaderGraph* graph);


private:
	bool grid = true;
	float2 scrollCoords = { 0.0f, 0.0f };
	std::string filter_popup = "";
	char node_filter[64];

	ResourceShader* current_shader = nullptr;
	bool creating_shader = false;
	bool selecting_shader = false;
};
