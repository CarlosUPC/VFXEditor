#pragma once

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"

#include "Globals.h"

class ShaderGraph;

enum NodeType
{
	PBR,
	UNKNOWN
};


class ShaderNode
{
public:
	ShaderNode();
	ShaderNode(const char* name, NodeType type, float2 position);
	~ShaderNode();

	void Draw(ShaderGraph& graph);
	virtual void InnerDraw(ShaderGraph& graph);

	float2 CalcNodePosition(ShaderGraph& graph, float2 pos = {});
	float2 CalcNodeSize(ShaderGraph& graph, ShaderNode* node);
public:
	NodeType type;
	float2 position;
	float2 size;
	float2 title_size;
	uint UID;
	std::string name;
};
