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

enum ValueType
{
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	NONE
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

struct InputNode
{
	ValueType type = ValueType::NONE;
	float2 position;
	std::string name;

	union
	{
		float value1;
		float2 value2;
		float3 value3;
		float4 value4;
	};

	 InputNode(){}
	 InputNode(const char* name, ValueType type)
	 {
		 this->type = type;
		 this->name = name;
	 }

};

struct OutputNode
{
	ValueType type = ValueType::NONE;
	float2 position;
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

	std::vector<InputNode> inputs;
	std::vector<OutputNode> outputs;
};
