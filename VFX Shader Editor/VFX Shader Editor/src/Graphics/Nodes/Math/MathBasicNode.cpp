#include "MathBasicNode.h"
#include "ShaderUniform.h"
#include "ShaderGraph.h"
#include "Application.h"


//=========================================================================================================================
// [NODE] ADD NODE
//=========================================================================================================================

AddNode::AddNode()
{
}

AddNode::AddNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("A", VALUE_TYPE::FLOAT1));
	inputs.push_back(InputSocket("B", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	inputs_size = 2;
	outputs_size = 1;
}

void AddNode::Update(ShaderGraph& graph)
{

	//Reset if the input is unlinked
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (!input.isLinked)
		{
			input.type = VALUE_TYPE::FLOAT1;
		}

		this->isError = false;

	}


	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID) + "_multiply";
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Variable declaration
	this->GLSL_Declaration = "";

	SetValuesByType(inputs[0].value4, inputs[0].data_str, inputs[0].type);
	SetValuesByType(inputs[1].value4, inputs[1].data_str, inputs[1].type);


	//Update connections
	CheckNodeConnections(this, graph);



	if (inputs[0].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[1].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else if (inputs[1].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[0].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);

	}
	else if (inputs[0].type == inputs[1].type)
	{

		if (inputs[0].type > inputs[1].type)
		{
			outputs[0].type = inputs[0].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}
		else
		{
			outputs[0].type = inputs[1].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else
	{
		this->isError = true;

		std::string log_text = std::string("[ERROR]: Arithmetic between type " + ShaderCompiler::SetOutputType(inputs[0].type) + " and " + ShaderCompiler::SetOutputType(inputs[1].type));

		LOG(log_text.c_str());
		std::cout << log_text.c_str() << std::endl;
	}
}

void AddNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			if (!input.isLinked)
				input.DisplayInputSocketDetails(graph, *this);
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());

	}
}

std::string AddNode::SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2)
{
	return SetType(outputs[0].type) + out_name + " = " + param1 + " + " + param2 + ";\n";
}


//=========================================================================================================================
// [NODE] SUBTRACT NODE
//=========================================================================================================================


SubtractNode::SubtractNode()
{
}

SubtractNode::SubtractNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("A", VALUE_TYPE::FLOAT1));
	inputs.push_back(InputSocket("B", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	inputs_size = 2;
	outputs_size = 1;
}

void SubtractNode::Update(ShaderGraph& graph)
{
	//Reset if the input is unlinked
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (!input.isLinked)
		{
			input.type = VALUE_TYPE::FLOAT1;
		}

		this->isError = false;

	}


	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID) + "_multiply";
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Variable declaration
	this->GLSL_Declaration = "";

	SetValuesByType(inputs[0].value4, inputs[0].data_str, inputs[0].type);
	SetValuesByType(inputs[1].value4, inputs[1].data_str, inputs[1].type);


	//Update connections
	CheckNodeConnections(this, graph);



	if (inputs[0].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[1].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else if (inputs[1].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[0].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);

	}
	else if (inputs[0].type == inputs[1].type)
	{

		if (inputs[0].type > inputs[1].type)
		{
			outputs[0].type = inputs[0].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}
		else
		{
			outputs[0].type = inputs[1].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else
	{
		this->isError = true;

		std::string log_text = std::string("[ERROR]: Arithmetic between type " + ShaderCompiler::SetOutputType(inputs[0].type) + " and " + ShaderCompiler::SetOutputType(inputs[1].type));

		LOG(log_text.c_str());
		std::cout << log_text.c_str() << std::endl;
	}
}

void SubtractNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			if (!input.isLinked)
				input.DisplayInputSocketDetails(graph, *this);
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());

	}
}

std::string SubtractNode::SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2)
{
	return SetType(outputs[0].type) + out_name + " = " + param1 + " - " + param2 + ";\n";
}


//=========================================================================================================================
// [NODE] MULTIPLY NODE
//=========================================================================================================================


MultiplyNode::MultiplyNode()
{
}

MultiplyNode::MultiplyNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{

	inputs.push_back(InputSocket("A", VALUE_TYPE::FLOAT1));
	inputs.push_back(InputSocket("B", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	inputs_size = 2;
	outputs_size = 1;

}

void MultiplyNode::Update(ShaderGraph& graph)
{
	//Reset if the input is unlinked
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (!input.isLinked)
		{
			input.type = VALUE_TYPE::FLOAT1;
		}

		this->isError = false;

	}


	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID) + "_multiply";
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Variable declaration
	this->GLSL_Declaration = "";

	SetValuesByType(inputs[0].value4, inputs[0].data_str, inputs[0].type);
	SetValuesByType(inputs[1].value4, inputs[1].data_str, inputs[1].type);


	//Update connections
	CheckNodeConnections(this, graph);



	if (inputs[0].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[1].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else if (inputs[1].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[0].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);

	}
	else if (inputs[0].type == inputs[1].type)
	{

		if (inputs[0].type > inputs[1].type)
		{
			outputs[0].type = inputs[0].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}
		else
		{
			outputs[0].type = inputs[1].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else
	{
		this->isError = true;

		std::string log_text = std::string("[ERROR]: Arithmetic between type " + ShaderCompiler::SetOutputType(inputs[0].type) + " and " + ShaderCompiler::SetOutputType(inputs[1].type));

		LOG(log_text.c_str());
		std::cout << log_text.c_str() << std::endl;
	}




}

void MultiplyNode::InspectorUpdate(ShaderGraph& graph)
{

	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			if (!input.isLinked)
				input.DisplayInputSocketDetails(graph, *this);
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());

	}

}

std::string MultiplyNode::SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2)
{
	return SetType(outputs[0].type) + out_name + " = " + param1 + " * " + param2 + ";\n";
}



//=========================================================================================================================
// [NODE] DIVIDE NODE
//=========================================================================================================================


DivideNode::DivideNode()
{

}

DivideNode::DivideNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("A", VALUE_TYPE::FLOAT1));
	inputs.push_back(InputSocket("B", VALUE_TYPE::FLOAT1));
	outputs.push_back(OutputSocket(VALUE_TYPE::FLOAT1));

	inputs_size = 2;
	outputs_size = 1;
}

void DivideNode::Update(ShaderGraph& graph)
{
	//Reset if the input is unlinked
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		InputSocket& input = this->inputs[i];

		if (!input.isLinked)
		{
			input.type = VALUE_TYPE::FLOAT1;
		}

		this->isError = false;

	}


	//Out Variable
	outputs[0].data_str = std::string(name) + std::to_string(UID) + "_multiply";
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Variable declaration
	this->GLSL_Declaration = "";

	SetValuesByType(inputs[0].value4, inputs[0].data_str, inputs[0].type);
	SetValuesByType(inputs[1].value4, inputs[1].data_str, inputs[1].type);


	//Update connections
	CheckNodeConnections(this, graph);



	if (inputs[0].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[1].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else if (inputs[1].type == VALUE_TYPE::FLOAT1)
	{
		outputs[0].type = inputs[0].type;
		outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);

	}
	else if (inputs[0].type == inputs[1].type)
	{

		if (inputs[0].type > inputs[1].type)
		{
			outputs[0].type = inputs[0].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}
		else
		{
			outputs[0].type = inputs[1].type;
			outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);
		}

		this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str);
	}
	else
	{
		this->isError = true;

		std::string log_text = std::string("[ERROR]: Arithmetic between type " + ShaderCompiler::SetOutputType(inputs[0].type) + " and " + ShaderCompiler::SetOutputType(inputs[1].type));

		LOG(log_text.c_str());
		std::cout << log_text.c_str() << std::endl;
	}
}

void DivideNode::InspectorUpdate(ShaderGraph& graph)
{

	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			InputSocket& input = this->inputs[i];

			if (!input.isLinked)
				input.DisplayInputSocketDetails(graph, *this);
		}
	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());

	}
}

std::string DivideNode::SetGLSLDefinition(const std::string& out_name, const std::string& param1, const std::string& param2)
{
	return SetType(outputs[0].type) + out_name + " = " + param1 + " / " + param2 + ";\n";
}
