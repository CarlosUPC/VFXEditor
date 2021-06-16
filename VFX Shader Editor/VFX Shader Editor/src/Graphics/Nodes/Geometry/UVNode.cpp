#include "UVNode.h"
#include "ShaderGraph.h"

UVNode::UVNode()
{
}

UVNode::UVNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("In", VALUE_TYPE::FLOAT2, float2(0.0f), CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT2));

	//temp hardcoded
	inputs_size = 1.5;
	outputs_size = 1;
}

void UVNode::Update(ShaderGraph& graph)
{
	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID);
	//Out Type
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Ins
	inputs[0].value_str = uv_str;
	
	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].value_str);
}

void UVNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
		uv_str = "TexCoord";

		ImGui::Text("Un Mirror U");
		ImGui::SameLine();
		ImGui::Checkbox("##unmirrorU", &mirrorU);

		if(mirrorU)
		{
			uv_str = "1.0 - TexCoord.x, TexCoord.y";
		}
		
		ImGui::Text("Un Mirror V");
		ImGui::SameLine();
		ImGui::Checkbox("##unmirrorV", &mirrorV);

		if(mirrorV)
		{
			uv_str = "TexCoord.x, 1.0 - TexCoord.y";
		}
		
		
		if (mirrorU && mirrorV)
		{
			uv_str = "1.0 - TexCoord.x, 1.0 - TexCoord.y";
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());

	}
}

std::string UVNode::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string("	vec2 " + out_name + ";\n");
}

std::string UVNode::SetGLSLDefinition(const std::string& out_name, const std::string& value)
{
	return std::string(out_name + " = " + "vec2(" + value + ");\n");
}
