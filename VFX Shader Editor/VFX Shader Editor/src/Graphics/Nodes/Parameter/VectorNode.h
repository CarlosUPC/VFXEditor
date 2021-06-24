#pragma once


#include "ShaderNode.h"
#include "CorlorPicker.h"

//=========================================================================================================================
// [NODE] VECTOR1 NODE
//=========================================================================================================================
class Vector1Node : public ShaderNode
{
public:
	Vector1Node();
	Vector1Node(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x);

private:
	//float output;
	//std::vector<std::string> input_code;
	//std::vector<std::string> output_code;
};

//=========================================================================================================================
// [NODE] VECTOR2 NODE
//=========================================================================================================================
class Vector2Node : public ShaderNode
{
public:
	Vector2Node();
	Vector2Node(const char* name, NODE_TYPE type, float2 position);
	
	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	
	
	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y);
	
private:
	
};

//=========================================================================================================================
// [NODE] VECTOR3 NODE
//=========================================================================================================================
class Vector3Node : public ShaderNode
{
public:
	Vector3Node();
	Vector3Node(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y, const std::string& value_z);


private:

};

//=========================================================================================================================
// [NODE] VECTOR4 NODE
//=========================================================================================================================
class Vector4Node : public ShaderNode
{
public:
	Vector4Node();
	Vector4Node(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& value_w);

private:

};


//=========================================================================================================================
// [NODE] COLOR NODE
//=========================================================================================================================
class ColorNode : public ShaderNode
{
public:
	ColorNode();
	ColorNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;


	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value);

public:
	ColorPicker picker;
	bool open;

};

//=========================================================================================================================
// [NODE] TIME NODE
//=========================================================================================================================
class TimeNode : public ShaderNode
{
public:
	TimeNode();
	TimeNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;


	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value);

public:
	

};