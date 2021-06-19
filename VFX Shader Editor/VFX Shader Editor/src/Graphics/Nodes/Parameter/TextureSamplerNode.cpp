#include "TextureSamplerNode.h"
#include "ShaderUniform.h"
#include "ShaderGraph.h"
#include "Application.h"

TextureSamplerNode::TextureSamplerNode()
{
}

TextureSamplerNode::TextureSamplerNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("UV", VALUE_TYPE::FLOAT2));
	inputs.push_back(InputSocket("Texture", VALUE_TYPE::TEXTURE2D));

	outputs.push_back(OutputSocket("RGBA", VALUE_TYPE::FLOAT4));
	outputs.push_back(OutputSocket("R", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket("G", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket("B", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket("A", VALUE_TYPE::FLOAT1));


	//temp hardcoded
	inputs_size = 5;
	outputs_size = 5;

	isUniform = true;
}

TextureSamplerNode::~TextureSamplerNode()
{
	
}

void TextureSamplerNode::Update(ShaderGraph& graph)
{
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];
		if (!input.isLinked) this->isError = false;
	}


	//Variable declaration
	this->GLSL_Declaration = SetGLSLDeclaration(std::string(name) + std::to_string(UID));

	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID) + "_sampler";
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
	
	outputs[1].data_str = std::string(name) + std::to_string(UID) + "_sampler.x";
	outputs[1].type_str = ShaderCompiler::SetOutputType(outputs[1].type);
	
	outputs[2].data_str = std::string(name) + std::to_string(UID) + "_sampler.y";
	outputs[2].type_str = ShaderCompiler::SetOutputType(outputs[2].type);
	
	outputs[3].data_str = std::string(name) + std::to_string(UID) + "_sampler.z";
	outputs[3].type_str = ShaderCompiler::SetOutputType(outputs[3].type);
	
	outputs[4].data_str = std::string(name) + std::to_string(UID) + "_sampler.w";
	outputs[4].type_str = ShaderCompiler::SetOutputType(outputs[4].type);


	//In Default Variable
	inputs[0].data_str = "TexCoord";
	inputs[1].data_str = std::string(name) + std::to_string(UID);

	//Check For Ins Variables
	CheckNodeConnections(this, graph);

	//Reassign the same data name because a uniform (TODO: improve that)
	inputs[1].data_str = std::string(name) + std::to_string(UID);

	//Variable definition (need inputs)
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[1].data_str, inputs[0].data_str);

}

void TextureSamplerNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//If node has inputs from other nodes, priorize their output values
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			if (input.isLinked && input.type == VALUE_TYPE::TEXTURE2D)
				return;
		}


		std::string item_name = std::string("     ") + graph.texIndices[0];
		static std::string current_item = item_name;

		
		static uint selected_idx = 0;

		if (ImGui::BeginCombo("##combo", current_item.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for (int idx = 0; idx < graph.texIndices.size(); idx++)
			{
				bool is_selected = (current_item == graph.texIndices[idx].c_str());
				auto drawList = ImGui::GetWindowDrawList();

				std::string item_name = std::string("     ") + graph.texIndices[idx];

				if (ImGui::Selectable(item_name.c_str(), is_selected)) {

					selected_idx = idx;
					current_item = std::string("     ") + graph.texIndices[idx];


					auto uniform = graph.uniforms.find(std::string(name) + std::to_string(UID));
					if (uniform != graph.uniforms.end())
					{
						static_cast<UniformTexture*>(uniform->second)->SetTextureID(App->textures[idx].handle);
					}

				}

				auto rect_min = ImGui::GetItemRectMin();
				auto rect_max = ImGui::GetItemRectMax();
				rect_max.x = rect_min.x + 32;
				drawList->AddImage((ImTextureID)App->textures[idx].handle, rect_min, rect_max, ImVec2(1, 1), ImVec2(0, 0), IM_COL32(255, 255, 255, 255));

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();


		}
		else
		{
			auto rect_min = ImGui::GetItemRectMin();
			auto rect_max = ImGui::GetItemRectMax();
			rect_max.x = rect_min.x + 32;
			auto drawList = ImGui::GetWindowDrawList();
			drawList->AddImage((ImTextureID)App->textures[selected_idx].handle, rect_min, rect_max, ImVec2(1, 1), ImVec2(0, 0), IM_COL32(255, 255, 255, 255));

		}

	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());
	}


}

std::string TextureSamplerNode::SetGLSLDeclaration(const std::string& out_name)
{
	return "uniform sampler2D " + out_name + ";\n";
}

std::string TextureSamplerNode::SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2)
{
	return "vec4 " + out_name  + " = " + "texture(" + param1 + ", " + param2 + ");\n";
}





TextureNode::TextureNode()
{
}

TextureNode::TextureNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("tex", VALUE_TYPE::TEXTURE2D, App->textures[0].handle, CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::TEXTURE2D));

	//temp hardcoded
	inputs_size = 3;
	outputs_size = 1;

	
}

void TextureNode::Update(ShaderGraph& graph)
{
	//Variable declaration
	//this->GLSL_Declaration = SetGLSLDeclaration(std::string(name) + std::to_string(UID));

	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID);
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
	
	//this->GLSL_Definition = "";


	//Ins Variable
	//inputs[0].values_str[0] = std::to_string(inputs[0].texid);

}

void TextureNode::InspectorUpdate(ShaderGraph& graph)
{

	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			input.DisplayInputSocketDetails(graph, *this);
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Declaration.c_str());

	}

}

std::string TextureNode::SetGLSLDeclaration(const std::string& out_name)
{
	return "uniform sampler2D " + out_name + ";\n";
}

