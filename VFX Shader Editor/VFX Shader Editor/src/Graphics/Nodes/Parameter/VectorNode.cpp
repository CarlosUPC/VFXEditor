#include "VectorNode.h"
#include <string>
#include "ShaderGraph.h"
#include "ShaderUniform.h"
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
	outputs[0].data_str = std::string(name) + std::to_string(UID);


	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
	
	//this->code_str = GLSLAbstactVarFloat(inputs[0].value_str, outputs[0].data_str);

	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].value_str);


	/*for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (input.type == VALUE_TYPE::FLOAT1)
		{
			input.input_str = std::to_string(input.value1);


			
		}
	}*/


}



void Vector1Node::InspectorUpdate(ShaderGraph& graph)
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
		ImGui::Text(GLSL_Definition.c_str());

	}
}



std::string Vector1Node::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string("float " + out_name + ";\n");
}

std::string Vector1Node::SetGLSLDefinition(const std::string& out_name, const std::string& value_x)
{
	return std::string(out_name + " = " + value_x + ";\n");
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
	//this->code_str = GLSLAbstactVarVec2(inputs[0].values_str[0], inputs[0].values_str[1], outputs[0].data_str);

	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].values_str[0], inputs[0].values_str[1]);
}

void Vector2Node::InspectorUpdate(ShaderGraph& graph)
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
		ImGui::Text(GLSL_Definition.c_str());

	}
}



std::string Vector2Node::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string("vec2 " + out_name + ";\n");
}

std::string Vector2Node::SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y)
{
	return std::string(out_name + " = " + "vec2(" + value_x + "," + value_y + ");\n");
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
	//this->code_str = GLSLAbstactVarVec3(inputs[0].values_str[0], inputs[0].values_str[1], inputs[0].values_str[2], outputs[0].data_str);

	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].values_str[0], inputs[0].values_str[1], inputs[0].values_str[2]);
}

void Vector3Node::InspectorUpdate(ShaderGraph& graph)
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
		ImGui::Text(GLSL_Definition.c_str());

	}
}



std::string Vector3Node::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string("vec3 " + out_name + ";\n");
}

std::string Vector3Node::SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y, const std::string& value_z)
{
	return std::string(out_name + " = " + "vec3(" + value_x + "," + value_y + "," + value_z + ");\n");
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
	//this->code_str = GLSLAbstactVarVec4(inputs[0].values_str[0], inputs[0].values_str[1], inputs[0].values_str[2], inputs[0].values_str[3], outputs[0].data_str);
	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].values_str[0], inputs[0].values_str[1], inputs[0].values_str[2], inputs[0].values_str[3]);
}

void Vector4Node::InspectorUpdate(ShaderGraph& graph)
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
		ImGui::Text(GLSL_Definition.c_str());

	}
}


std::string Vector4Node::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string("vec4 " + out_name + ";\n");
}

std::string Vector4Node::SetGLSLDefinition(const std::string& out_name, const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& value_w)
{
	return std::string(out_name + " = " + "vec4(" + value_x + "," + value_y + "," + value_z + "," + value_w + ");\n");
}



ColorNode::ColorNode()
{
}

ColorNode::ColorNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{

	inputs.push_back(InputSocket("In", VALUE_TYPE::COLOR3, float3(0.0f), CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT4));

	//temp hardcoded
	inputs_size = 1.5;
	outputs_size = 1;

}

void ColorNode::Update(ShaderGraph& graph)
{
	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID);
	//Out Type
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Ins
	inputs[0].value_str = std::to_string(inputs[0].value3.x) + "," + std::to_string(inputs[0].value3.y) + "," + std::to_string(inputs[0].value3.z);

	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].value_str);
}

void ColorNode::InspectorUpdate(ShaderGraph& graph)
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
		ImGui::Text(GLSL_Definition.c_str());

	}
}

std::string ColorNode::SetGLSLDeclaration(const std::string& out_name)
{
	return std::string("vec4 " + out_name + ";\n");
}

std::string ColorNode::SetGLSLDefinition(const std::string& out_name, const std::string& value)
{
	return std::string(out_name + " = " + "vec4(" + value + ", 1.0);\n");
}

TimeNode::TimeNode()
{
}

TimeNode::TimeNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("Time", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::PARAMETER));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	//temp hardcoded
	inputs_size = 1.35;
	outputs_size = 1;

	isUniform = true;
}

void TimeNode::Update(ShaderGraph& graph)
{
	//Outs
	outputs[0].data_str = std::string(name) + std::to_string(UID);
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Ins
	inputs[0].data_str = std::to_string(graph.GetTimeSinceLastCompilation());


	//Update input value (time)
	auto uniform = graph.uniforms.find(outputs[0].data_str);
	if (uniform != graph.uniforms.end())
	{
		static_cast<UniformFloat*>(uniform->second)->SetValue(graph.GetTimeSinceLastCompilation());
	}


	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str);
}

void TimeNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float timer;

		auto uniform = graph.uniforms.find(outputs[0].data_str);
		if (uniform != graph.uniforms.end())
		{
			timer = static_cast<UniformFloat*>(uniform->second)->GetValue();
			ImGui::InputFloat("Time", &timer, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
		}
	}



}

std::string TimeNode::SetGLSLDeclaration(const std::string& out_name)
{
	return "uniform float " + out_name + ";\n";
}

std::string TimeNode::SetGLSLDefinition(const std::string& out_name, const std::string& value)
{
	return std::string();
}
