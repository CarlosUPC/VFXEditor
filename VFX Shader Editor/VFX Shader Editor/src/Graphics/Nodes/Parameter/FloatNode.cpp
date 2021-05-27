#include "FloatNode.h"
#include <string>

FloatNode::FloatNode()
{
}

FloatNode::FloatNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("x", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	//temp hardcoded
	inputs_size = 1.2;
	outputs_size = 1;

}

void FloatNode::Update(ShaderGraph& graph)
{

	inputs[0].input_str = std::to_string(inputs[0].value1);
	outputs[0].output_str = std::string(name + "_out");
	outputs[0].type_str = std::string("float");
	
	this->code_str = std::string("float " + outputs[0].output_str + "=" + inputs[0].input_str + ";\n");

	/*for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (input.type == VALUE_TYPE::FLOAT1)
		{
			input.input_str = std::to_string(input.value1);


			
		}
	}*/


}



void FloatNode::InspectorUpdate()
{
	
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			input.DisplayInputSocketDetails();
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(code_str.c_str());

	}
}
