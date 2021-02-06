#pragma once

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\imgui_internal.h"

#include "Globals.h"

class ShaderGraph;

enum NodeType
{
	PBR,
	UNKNOWN
};

struct ActionNode
{
	enum StateType
	{
		DRAG_NODE,
		APPLY_DRAG,
		NONE
	};

	StateType type = StateType::NONE;
	float2 mouse_pos;


};

class ShaderNode
{
public:
	ShaderNode();
	ShaderNode(const char* name, NodeType type, float2 position);
	~ShaderNode();

	void Input(ShaderGraph& graph);
	void Draw(ShaderGraph& graph);
	virtual void InnerDraw(ShaderGraph& graph);

	float2 CalcNodePosition(ShaderGraph& graph, float2 pos = {});
	float2 CalcNodeSize(ShaderGraph& graph, ShaderNode* node);

	bool NodeHovering(ShaderGraph& graph, float2 position, float2 size);
public:
	NodeType type;
	float2 position;
	float2 size;
	float2 title_size;
	uint UID;
	std::string name;

	bool isHovered = false;

	ActionNode action;
};
