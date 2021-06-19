#pragma once

#include "Panel.h"
#include <functional>

class ResourceShader;
class ShaderNode;
class ShaderGraph;
enum NODE_TYPE;


typedef ShaderNode* (ShaderGraph::* CreateNodeFn)(const char* n, int t);
typedef std::function<ShaderNode * (const char*, int)> create_node;

struct Canvas
{
	friend class PanelShaderEditor;
public:
	//Canvas();
	//~Canvas();

	void Init(ImVec2 m_Pos, ImVec2 m_Size, ImColor m_Color);
	void Draw(ImDrawList* draw_list, ImVec2 m_Pos, ImVec2 m_Size);
private:
	ImVec2 m_Pos;
	ImVec2 m_Size;
	ImColor m_Color;

	float grid_size = 16.0f;
	bool show_grid = true;

protected:
	float2 m_Scroll;


};



class PanelShaderEditor : public Panel
{
	//friend class Canvas;
public:
	

	PanelShaderEditor(const char* name);
	~PanelShaderEditor();

	void Draw();
	void OnShaderAction(std::string& action);
	void CreateShaderContext();
	void LoadShaderContext();
	void AddNewNodePopUp();

	void AddNode(ShaderGraph& graph, ShaderNode* node);

	ShaderNode CreateNode();

	void NodeOption(bool& open_menu, const char* name, NODE_TYPE type, float2 position, ResourceShader* shader, ShaderGraph* graph, ShaderNode* (ShaderGraph::*p)(const char* n, int t, float2 p));

	void NodeOption(const char* name, NODE_TYPE type, ResourceShader* shader, ShaderGraph* graph, bool& open_menu);
	void CreateNodeOption(const char* name, NODE_TYPE type, float2 position, ResourceShader* shader, ShaderGraph* graph);
	ShaderGraph* GetGraph();

	void Scrolling();
	void NodeContext();

private:
	bool grid = true;
	float2 scrollCoords = { 0.0f, 0.0f };
	std::string filter_popup = "";
	char node_filter[64];

	ResourceShader* current_shader = nullptr;
	bool creating_shader = false;
	bool selecting_shader = false;

	ImFont* textFont;
	ImFont* mainFont;

	Canvas canvas;
	ImVec2 hit_pos;

};
