#pragma once

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\imgui_internal.h"

#include "Globals.h"

class ShaderGraph;
class ShaderNode;
struct InputSocket;
struct OutputSocket;

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



struct Connector
{
	

	ShaderNode* to = nullptr;
	ShaderNode* from = nullptr;
	unsigned int index_in = 0;
	unsigned int index_out = 0;

	void DrawConnector(ShaderGraph& g, bool isInput = false);

	void DrawInputChannel(ShaderGraph& g, InputSocket& input);
	void DrawOutputChannel(ShaderGraph& g, OutputSocket& output);

	void AddBezierLine(ShaderGraph& g, float2 start, float2 end, bool isLinked = true);

	bool ConnectorHovering(float2 position, float2 size);
};


struct InputSocket
{
	ValueType type = ValueType::NONE;
	float2 position;
	std::string name;
	Connector connector;

	union
	{
		float value1;
		float2 value2;
		float3 value3;
		float4 value4;
	};

	 InputSocket(){}
	 InputSocket(const char* name, ValueType type)
	 {
		 this->type = type;
		 this->name = name;

		 value1 = 5.f;
	 }

};

struct OutputSocket
{
	ValueType type = ValueType::NONE;
	float2 position;
	Connector connector;
	std::string name;

	OutputSocket(){}
	OutputSocket(ValueType type)
	{
		this->type = type;
	}

	OutputSocket(const char* name, ValueType type)
	{
		this->type = type;
		this->name = name;
	}
};



class ShaderNode
{
	
public:
	ShaderNode();
	ShaderNode(const char* name, NodeType type, float2 position);
	~ShaderNode();

	void InputNode(ShaderGraph& graph);
	void DrawNode(ShaderGraph& graph);
	virtual void Update(ShaderGraph& graph);

	float2 CalcNodePosition(ShaderGraph& graph, float2 pos = {});
	float2 CalcNodeSize(ShaderGraph& graph, ShaderNode* node);

	bool NodeHovering(ShaderGraph& graph, float2 position, float2 size);
	bool ConnectorHovering(float2 position, float2 size);

	void DrawTitle(ShaderGraph& g);
	void DrawTitle(ImDrawList* draw_list, float2 pos, float2 size);
	void DrawBody(ImDrawList* draw_list, float2 pos, float2 size);
	void DrawInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset = 0);
	void DrawOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset = 0);

	void DrawInputConnector(ShaderGraph& graph, InputSocket& input, unsigned int index = 0);
	void DrawOutputConnector(ShaderGraph& graph, OutputSocket& output, unsigned int index = 0);
public:
	NodeType type;
	float2 position;
	float2 size;
	float2 title_size;
	uint UID;
	std::string name;


	//ActionNode action;

	std::vector<InputSocket> inputs;
	std::vector<OutputSocket> outputs;

	int inputs_count = 0;
	int outputs_count = 0;

	//node states
	bool isHovered = false;
	bool isSelected = false;
	bool isItemActive = false;

};
