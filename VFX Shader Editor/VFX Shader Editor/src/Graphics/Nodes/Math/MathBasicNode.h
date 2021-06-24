#pragma once

#include "ShaderNode.h"

//=========================================================================================================================
// [NODE] ADD NODE
//=========================================================================================================================
class AddNode : public ShaderNode
{
public:
	AddNode();
	AddNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2);
private:
	//float output;
	//std::vector<std::string> input_code;
	//std::vector<std::string> output_code;
};

//=========================================================================================================================
// [NODE] SUBTRACT NODE
//=========================================================================================================================
class SubtractNode : public ShaderNode
{
public:
	SubtractNode();
	SubtractNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2);
private:
	/*float output;
	std::vector<std::string> input_code;
	std::vector<std::string> output_code;*/
};


//=========================================================================================================================
// [NODE] MULTIPLY NODE
//=========================================================================================================================
class MultiplyNode : public ShaderNode
{
public:
	MultiplyNode();
	MultiplyNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2);
private:
	/*float output;
	std::vector<std::string> input_code;
	std::vector<std::string> output_code;*/
};

//=========================================================================================================================
// [NODE] DIVIDE NODE
//=========================================================================================================================
class DivideNode : public ShaderNode
{
public:
	DivideNode();
	DivideNode(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2);
private:
	/*float output;
	std::vector<std::string> input_code;
	std::vector<std::string> output_code;*/
};