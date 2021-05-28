#include "VectorNode.h"
#include <string>
#include "ShaderGraph.h"
//#include "MathGeoLib/Math/float4.h"

Vector1Node::Vector1Node()
{
}

Vector1Node::Vector1Node(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("x", VALUE_TYPE::FLOAT1, 0.0f, CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	//temp hardcoded
	inputs_size = 1.5;
	outputs_size = 1;

}

void Vector1Node::Update(ShaderGraph& graph)
{

	inputs[0].value_str = std::to_string(inputs[0].value1);
	outputs[0].data_str = std::string(name + "_out");


	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
	
	this->code_str = ShaderCompiler::SetOutputVarFloat(inputs[0].value_str, outputs[0].data_str);
	
	/*for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (input.type == VALUE_TYPE::FLOAT1)
		{
			input.input_str = std::to_string(input.value1);


			
		}
	}*/


}



void Vector1Node::InspectorUpdate()
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

Vector2Node::Vector2Node()
{
}

Vector2Node::Vector2Node(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("xy", VALUE_TYPE::FLOAT2, float2(0.0f), CONTEXT_TYPE::PARAMETER));
	//inputs.push_back(InputSocket("y", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT2));

	//temp hardcoded
	inputs_size = 1.5;
	outputs_size = 1;
}

void Vector2Node::Update(ShaderGraph& graph)
{
	//In Values
	inputs[0].values_str[0] = std::to_string(inputs[0].value2.x);
	inputs[0].values_str[1] = std::to_string(inputs[0].value2.y);

	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID);
	
	//Out Type
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Abstract to GLSL code
	this->code_str = ShaderCompiler::SetOutputVarVector2(inputs[0].values_str[0], inputs[0].values_str[1], outputs[0].data_str);
}

void Vector2Node::InspectorUpdate()
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

Vector3Node::Vector3Node()
{
}

Vector3Node::Vector3Node(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("xyz", VALUE_TYPE::FLOAT3, float3(0.0f), CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT3));
	
	//temp hardcoded
	inputs_size = 1.5;
	outputs_size = 1;
}

void Vector3Node::Update(ShaderGraph& graph)
{
	//In Values
	inputs[0].values_str[0] = std::to_string(inputs[0].value3.x);
	inputs[0].values_str[1] = std::to_string(inputs[0].value3.y);
	inputs[0].values_str[2] = std::to_string(inputs[0].value3.z);

	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID);

	//Out Type
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Abstract to GLSL code
	this->code_str = ShaderCompiler::SetOutputVarVector3(inputs[0].values_str[0], inputs[0].values_str[1], inputs[0].values_str[2], outputs[0].data_str);
}

void Vector3Node::InspectorUpdate()
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

Vector4Node::Vector4Node()
{
}

Vector4Node::Vector4Node(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("xyzw", VALUE_TYPE::FLOAT4, float4(0.0f, 0.0f, 0.0f, 0.0f), CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT4));

	//temp hardcoded
	inputs_size = 1.5;
	outputs_size = 1;
}

void Vector4Node::Update(ShaderGraph& graph)
{
	//In Values
	inputs[0].values_str[0] = std::to_string(inputs[0].value4.x);
	inputs[0].values_str[1] = std::to_string(inputs[0].value4.y);
	inputs[0].values_str[2] = std::to_string(inputs[0].value4.z);
	inputs[0].values_str[3] = std::to_string(inputs[0].value4.w);

	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID);

	//Out Type
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Abstract to GLSL code
	this->code_str = ShaderCompiler::SetOutputVarVector4(inputs[0].values_str[0], inputs[0].values_str[1], inputs[0].values_str[2], inputs[0].values_str[3], outputs[0].data_str);
}

void Vector4Node::InspectorUpdate()
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
