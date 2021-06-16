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
	inputs[0].data_str = uv_str;
	
	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str);
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
	return std::string(out_name + " = "  + value + ";\n");
}





TilingOffsetNode::TilingOffsetNode()
{
}

TilingOffsetNode::TilingOffsetNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{

	inputs.push_back(InputSocket("UV", VALUE_TYPE::FLOAT2));
	inputs.push_back(InputSocket("Tiling", VALUE_TYPE::FLOAT2));
	inputs.push_back(InputSocket("Offset", VALUE_TYPE::FLOAT2));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT2));

	//temp hardcoded
	inputs_size = 3;
	outputs_size = 1;
}

void TilingOffsetNode::Update(ShaderGraph& graph)
{

	//Outs
	outputs[0].data_str = "TilingAndOffset" + std::to_string(UID);
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Ins
	inputs[0].data_str = "TexCoord";
	inputs[1].data_str = "vec2(" + std::to_string(inputs[1].value2.x) + " , " + std::to_string(inputs[1].value2.y) + ")";;
	inputs[2].data_str = "vec2(" + std::to_string(inputs[2].value2.x) + " , " + std::to_string(inputs[2].value2.y) + ")";;

	//Check For Ins Variables
	CheckNodeConnections(this, graph);

	//GLSL Abstraction
	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str, inputs[2].data_str);


}

void TilingOffsetNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//Tiling ------------

		if (!inputs[1].isLinked)
		{
			ImGui::PushID("##X");
			ImGui::Text("Tiling U: "); ImGui::SameLine();
			ImGui::DragFloat("##tilingU", &inputs[1].value2.x, 0.1f, 0.0f, 9999999.0f, "%.2f");
			ImGui::PopID();

			ImGui::PushID("##Y");
			ImGui::Text("Tiling V: "); ImGui::SameLine();
			ImGui::DragFloat("##tilingV", &inputs[1].value2.y, 0.1f, 0.0f, 9999999.0f, "%.2f");
			ImGui::PopID();
		}


		ImGui::Separator();


		//Offset -------------

		if (!inputs[2].isLinked)
		{
			ImGui::PushID("##X");
			ImGui::Text("Offset U: "); ImGui::SameLine();
			ImGui::DragFloat("##offsetU", &inputs[2].value2.x, 0.1f, 0.0f, 9999999.0f, "%.2f");
			ImGui::PopID();

			ImGui::PushID("##Y");
			ImGui::Text("Offset V: "); ImGui::SameLine();
			ImGui::DragFloat("##offsetV", &inputs[2].value2.y, 0.1f, 0.0f, 9999999.0f, "%.2f");
			ImGui::PopID();
		}
		
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());
	}
}

std::string TilingOffsetNode::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string();
}

std::string TilingOffsetNode::SetGLSLDefinition(const std::string& out_name, const std::string& uv, const std::string& tiling, const std::string& offset)
{
	return std::string("	vec2 " + out_name + " = " + uv + " * " + tiling + " + " + offset + ";\n");
}
