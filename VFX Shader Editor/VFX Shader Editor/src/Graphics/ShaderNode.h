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


struct ShaderLink
{
	ShaderLink(ShaderNode* input_node, unsigned int input_socket, ShaderNode* output_node, unsigned int output_socket);


	ShaderNode* output_node = nullptr;
	ShaderNode* input_node = nullptr;
	unsigned int input_socket = 0;
	unsigned int output_socket = 0;

	//bool isLinked = false;
	bool to_delete = false;

	void DrawLink(ShaderGraph& graph);


};





struct InputSocket
{
	ValueType type = ValueType::NONE;
	float2 position;
	std::string name;
	

	bool isLinked = false;
	ShaderLink* link_ref = nullptr;

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
	
	std::string name;

	bool isLinked = false;
	ShaderLink* link_ref = nullptr;

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
	void DrawLinks(ShaderGraph& graph);
	virtual void Update(ShaderGraph& graph);

	float2 CalcNodePosition(ShaderGraph& graph, float2 pos = {});
	float2 CalcNodeSize(ShaderGraph& graph, ShaderNode* node);

	bool NodeHovering(ShaderGraph& graph, float2 position, float2 size);
	bool ConnectorHovering(float2 position, float2 size);
	bool SocketHovering(float2& p, float2 e,const float r1,const float r2);

	void DrawTitle(ShaderGraph& g);
	void DrawTitle(ImDrawList* draw_list, float2 pos, float2 size);
	void DrawBody(ImDrawList* draw_list, float2 pos, float2 size);
	void DrawInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset = 0);
	void DrawOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset = 0);

	void DrawInputConnector(ShaderGraph& graph, InputSocket& input, unsigned int index = 0);
	void DrawOutputConnector(ShaderGraph& graph, OutputSocket& output, unsigned int index = 0);

	void InputSocketInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset = 0);
	void InputSocketOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset = 0);
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

	//Links
	std::vector<ShaderLink> links;

	int inputs_count = 0;
	int outputs_count = 0;

	//node states
	bool isHovered = false;
	bool isSelected = false;
	bool isItemActive = false;

	bool isSocketHovered = false;

	bool	m_is_node_widgets_active;
	
	bool	m_is_old_any_active;

};
