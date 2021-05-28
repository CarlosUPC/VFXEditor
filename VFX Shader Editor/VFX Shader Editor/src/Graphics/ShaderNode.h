#pragma once

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\imgui_internal.h"


#include "Globals.h"

class ShaderGraph;
class ShaderCompiler;
class ShaderNode;
struct InputSocket;
struct OutputSocket;

enum NODE_TYPE
{
	VECTOR1,
	VECTOR2,
	VECTOR3,
	VECTOR4,
	PBR,

	UNKNOWN
};

enum VALUE_TYPE
{
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	NONE
};

enum class CONTEXT_TYPE
{
	PARAMETER  = 0,
	FUNCTION,
	
	NONE
};


struct ShaderLink
{

public:
	ShaderLink(ShaderNode* input_node, unsigned int input_socket, ShaderNode* output_node, unsigned int output_socket);

	bool LineHovering(float2 p1, float2 p2, const float r1, const float r2);

	void DrawLink(ShaderGraph& graph);
	void InputLink(ShaderGraph& graph);
	
	
public:
	//core info
	ShaderNode* output_node = nullptr;
	ShaderNode* input_node = nullptr;
	unsigned int input_socket = 0;
	unsigned int output_socket = 0;
	uint UID;

	//link states
	bool to_delete = false;
	bool isLineHovered = false;
	ShaderLink* link_selected = nullptr;
	


};





struct InputSocket
{
	VALUE_TYPE type = VALUE_TYPE::NONE;
	CONTEXT_TYPE context_type = CONTEXT_TYPE::NONE;

	float2 position;
	std::string name;
	std::string value_str;
	std::vector<std::string> values_str;
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
	 InputSocket(const char* name, VALUE_TYPE type)
	 {
		 this->type = type;
		 this->name = name;
		 
		 value1 = 5.f;
	 }

	 InputSocket(const char* name, VALUE_TYPE type, CONTEXT_TYPE context)
	 {
		 this->type = type;
		 this->name = name;
		 this->context_type = context;

		 value1 = 5.f;
	 }

	 InputSocket(const char* name, VALUE_TYPE type, float val, CONTEXT_TYPE context)
	 {
		 this->type = type;
		 this->name = name;
		 this->context_type = context;
		 this->value1 = val;
	 }

	 InputSocket(const char* name, VALUE_TYPE type, float2 val, CONTEXT_TYPE context)
	 {
		 this->type = type;
		 this->name = name;
		 this->context_type = context;
		 this->value2 = val;

		 this->values_str.push_back("");
		 this->values_str.push_back("");
	 }

	 InputSocket(const char* name, VALUE_TYPE type, float3 val, CONTEXT_TYPE context)
	 {
		 this->type = type;
		 this->name = name;
		 this->context_type = context;
		 this->value3 = val;

		 this->values_str.push_back("");
		 this->values_str.push_back("");
		 this->values_str.push_back("");
	 }

	 InputSocket(const char* name, VALUE_TYPE type, float4 val, CONTEXT_TYPE context)
	 {
		 this->type = type;
		 this->name = name;
		 this->context_type = context;
		 this->value4 = val;

		 this->values_str.push_back("");
		 this->values_str.push_back("");
		 this->values_str.push_back("");
		 this->values_str.push_back("");
	 }

	 void DisplayInputSocketDetails();

};

struct OutputSocket
{
	VALUE_TYPE type = VALUE_TYPE::NONE;
	float2 position;
	
	std::string name;
	std::string data_str;
	std::string type_str;

	bool isLinked = false;

	//TODO: MAKE IT VECTOR 
	ShaderLink* link_ref = nullptr;

	OutputSocket(){}
	OutputSocket(VALUE_TYPE type)
	{
		this->type = type;
	}

	OutputSocket(const char* name, VALUE_TYPE type)
	{
		this->type = type;
		this->name = name;
	}
};



class ShaderNode
{
	
public:
	ShaderNode();
	ShaderNode(const char* name, NODE_TYPE type, float2 position);
	~ShaderNode();

	virtual void Update(ShaderGraph& graph);
	virtual void InspectorUpdate();

	float2 CalcNodePosition(ShaderGraph& graph, float2 pos = {});
	float2 CalcNodeSize(ShaderGraph& graph, ShaderNode* node);

	bool NodeHovering(ShaderGraph& graph, float2 position, float2 size);
	bool ConnectorHovering(float2 position, float2 size);
	bool SocketHovering(float2& p1, float2 p2,const float r1,const float r2);

	void DrawLines(ShaderGraph& graph);
	void DrawNode(ShaderGraph& graph);
	void DrawTitle(ShaderGraph& g);
	void DrawTitle(ImDrawList* draw_list, float2 pos, float2 size);
	void DrawBody(ImDrawList* draw_list, float2 pos, float2 size);
	void DrawInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset = 0);
	void DrawOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset = 0);
	void DrawInputConnector(ShaderGraph& graph, InputSocket& input, unsigned int index = 0);
	void DrawOutputConnector(ShaderGraph& graph, OutputSocket& output, unsigned int index = 0);

	void InputNode(ShaderGraph& graph);
	void InputSocketInputs(ShaderGraph& graph, unsigned int numInputs, unsigned int offset = 0);
	void InputSocketOutputs(ShaderGraph& graph, unsigned int numOutputs, unsigned int offset = 0);

	InputSocket GetInputSocketbyName(const std::string& inputName);

	std::string GetOutputDefinition() { return code_str; }
public:
	//core info
	NODE_TYPE type;
	float2 position;
	float2 size;
	float2 title_size;
	uint UID;
	std::string name;

	//inputs & outputs
	std::vector<InputSocket> inputs;
	std::vector<OutputSocket> outputs;

	//node states
	bool isHovered = false;
	bool isSelected = false;
	bool isItemActive = false;
	bool isSocketHovered = false;
	bool to_delete = false;

	//temp
	float inputs_size = 0;
	float outputs_size = 0;

protected:
	std::string code_str;

};
