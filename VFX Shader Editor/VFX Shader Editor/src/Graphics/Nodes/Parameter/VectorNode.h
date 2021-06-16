#pragma once


#include "ShaderNode.h"
#include "CorlorPicker.h"

class Vector1Node : public ShaderNode
{
public:
	Vector1Node();
	Vector1Node(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	std::string GLSLAbstactVarFloat(const std::string& value, const std::string& name);

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x);

private:
	float output;
	std::vector<std::string> input_code;
	std::vector<std::string> output_code;
};


class Vector2Node : public ShaderNode
{
public:
	Vector2Node();
	Vector2Node(const char* name, NODE_TYPE type, float2 position);
	
	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	std::string GLSLAbstactVarVec2(const std::string& value_x, const std::string& value_y, const std::string& name);
	
	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y);
	
private:
	
};


class Vector3Node : public ShaderNode
{
public:
	Vector3Node();
	Vector3Node(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	std::string GLSLAbstactVarVec3(const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& name);

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y, const std::string& value_z);


private:

};


class Vector4Node : public ShaderNode
{
public:
	Vector4Node();
	Vector4Node(const char* name, NODE_TYPE type, float2 position);

	void Update(ShaderGraph& graph) override;
	void InspectorUpdate(ShaderGraph& graph) override;
	
	std::string GLSLAbstactVarVec4(const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& value_w, const std::string& name);

	std::string SetGLSLDeclaration(const std::string& out_name);
	std::string SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& value_w);

private:

};



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
	bool open = false;
	ImGuiColorEditFlags m_edit_flags;

};