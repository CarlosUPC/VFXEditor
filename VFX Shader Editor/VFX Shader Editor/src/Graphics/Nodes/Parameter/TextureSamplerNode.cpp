#include "TextureSamplerNode.h"

TextureSamplerNode::TextureSamplerNode()
{
}

TextureSamplerNode::TextureSamplerNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("Texture", VALUE_TYPE::TEXTURE2D));
	inputs.push_back(InputSocket("UV", VALUE_TYPE::FLOAT2));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT4));

	//temp hardcoded
	inputs_size = 3;
	outputs_size = 1;
}

void TextureSamplerNode::Update(ShaderGraph& graph)
{

	//Variable declaration
	this->GLSL_Declaration = SetGLSLDeclaration(std::string(name) + std::to_string(UID));

	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID) + "_sampler";


	//In Default Variable
	inputs[0].data_str = std::string(name) + std::to_string(UID);
	inputs[1].data_str = "TexCoord";

	//Check For Ins Variables
	CheckNodeConnections(this);

	//Variable definition (need inputs)
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);

}

void TextureSamplerNode::InspectorUpdate()
{
}

std::string TextureSamplerNode::SetGLSLDeclaration(const std::string& out_name)
{
	return "	uniform sampler2D " + out_name + ";\n";
}

std::string TextureSamplerNode::SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2)
{
	return "vec4 " + out_name  + " = " + "texture(" + param1 + ", " + param2 + ");\n";
}
