#include "ShaderGraph.h"
#include "ResourceShader.h"
#include <fstream>
//#include <sstream> 
#include <iostream>
#include "ShaderNode.h"
#include "Nodes/PBRNode.h"
ShaderGraph::ShaderGraph(std::string m_Name)
	:m_Name(m_Name)
{
}

ShaderGraph::~ShaderGraph()
{
}

void ShaderGraph::Draw()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.5 * 1, 2.5 * 1));
	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
	
	for (std::list<ShaderLink*>::reverse_iterator it = links.rbegin(); it != links.rend(); ++it) // Interaction in reverse loop because the last one drawn shoud be the first to be interacted
	{
		//Mouse Input stuff -----
		(*it)->InputLink(*this);

	}


	for (std::list<ShaderLink*>::iterator it = links.begin(); it != links.end(); ++it)
	{
		//Draw stuff -----
		(*it)->DrawLink(*this);

	}


	for (std::list<ShaderNode*>::reverse_iterator it = nodes.rbegin(); it != nodes.rend(); ++it) // Interaction in reverse loop because the last one drawn shoud be the first to be interacted
	{

		ImGui::PushID((*it)->UID);

		//Mouse Input stuff ------
		(*it)->InputNode(*this);
		(*it)->InputSocketInputs(*this, (*it)->inputs_count);
		(*it)->InputSocketOutputs(*this, (*it)->outputs_count);

		ImGui::PopID();
	}


	for (std::list<ShaderNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		ImGui::PushID((*it)->UID);

		//Draw stuff -------
		(*it)->DrawLines(*this);
		(*it)->DrawNode(*this);
		(*it)->DrawInputs(*this, (*it)->inputs_count);
		(*it)->DrawOutputs(*this, (*it)->outputs_count);

		//Update stuff -------
		(*it)->Update(*this);

		
		ImGui::PopID();
		
	}






	for (std::list<ShaderNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		//Delete stuff
		if ((*it)->to_delete == true)
		{

			//chack if node's sockets have links
			for (auto&& input : (*it)->inputs)
			{
				if (input.link_ref != nullptr)
				{
					input.link_ref->to_delete = true;
					input.link_ref->input_node = nullptr;
				}
			}
			for (auto&& output : (*it)->outputs)
			{
				if (output.link_ref != nullptr)
				{
					output.link_ref->to_delete = true;
					output.link_ref->output_node = nullptr;
				}
			}

			//delete node
			RELEASE((*it));
			it = nodes.erase(it);
		}

	}


	for (std::list<ShaderLink*>::iterator it = links.begin(); it != links.end(); ++it)
	{
		//Remove stuff ----
		if ((*it)->to_delete == true)
		{
			if((*it)->input_node != nullptr)
				(*it)->input_node->inputs[(*it)->input_socket].isLinked = false;

			if ((*it)->output_node != nullptr)
				(*it)->output_node->outputs[(*it)->output_socket].isLinked = false;

			RELEASE((*it));
			it = links.erase(it);
		}
	}

	


	

	//ImGui::PopStyleVar(2);
}

ShaderNode* ShaderGraph::CreateNode(const char* name, int type, float2 position)
{
	ShaderNode* node = nullptr;

	switch (type)
	{
	case NodeType::PBR:
		node = new PBRNode(name, (NodeType)type, position);
		break;
	case NodeType::UNKNOWN:
		break;

	}

	return node;
}

void ShaderGraph::CompileShader(ResourceShader* shader)
{
	static std::string last_output;

	ShaderCompiler compiler(*(shader->graph));
	compiler.Generate();

	if (last_output == compiler.source)
		return;

	//Recompile new shader
	shader->Recompile();

	last_output = std::string(compiler.source);
}

void ShaderGraph::SetScrollOffset(float2& offset)
{
	this->scrolling = offset;
}

ShaderCompiler::ShaderCompiler( ShaderGraph& g)
	: graph(g)
{
	source = "";
}

void ShaderCompiler::Generate()
{
	//Compile and modify the shadergraph reference

	//Vertex Shader Output
	source += OutputVertexHeader();
	source += BeginVertex();
	source += OutputVertex();
	source += EndVertex();

	//Fragment Shader Output
	source += OutputFragmentHeader();
	source += BeginFragment();
	source += OutputFragment();
	source += EndFragment();

	//Serialize shader to file
	WriteShaderToFile();

	//Create resource shader
	//ResourceShader* shader = new ResourceShader(SplitShaderSource(ShaderType::VERTEX).c_str(), SplitShaderSource(ShaderType::VERTEX).c_str());


}

void ShaderCompiler::WriteShaderToFile()
{

	
	std::string shader_path = std::string("Shaders");
	std::string vertName = graph.m_Name + std::string(".Vertex.glsl");
	std::string fragName = graph.m_Name + std::string(".Fragment.glsl");

	
	std::ofstream file;
	file.open((shader_path + "/" + vertName).c_str());
	
	if (file)
	{
		std::string code = SplitShaderSource(ShaderType::VERTEX);
		file.write(code.c_str(), code.length());

	}
	
	file.close();

	file.open((shader_path + "/" + fragName).c_str());
	
	if (file)
	{
		std::string code2 = SplitShaderSource(ShaderType::FRAGMENT);
		file.write(code2.c_str(), code2.length());
	}
	
	file.close();


}

std::string ShaderCompiler::SplitShaderSource(ShaderType type)
{
	
	switch (type)
	{
		case ShaderType::VERTEX:
		{
			std::string vertex_code = "";
			vertex_code = ParseFromTo(BeginVertexHeader(), EndVertexHeader(), source);
			return vertex_code;
			

		}break;

		case ShaderType::FRAGMENT:
		{
			std::string frag_code = "";
			frag_code = ParseFromTo(BeginFragmentHeader(), EndFragmentHeader(), source);
			return frag_code;
			

		}break;

		default:
		{
			std::string unknown_code = "";
			return unknown_code;

		}break;
	}
}

std::string ShaderCompiler::ParseFromTo(const std::string& begin, const std::string& end, const std::string& source)
{
	std::string returnStr = "";

	// Search for line to match
	std::size_t foundBegin = source.find(begin);
	std::size_t foundEnd = source.find(end);

	// Found
	if (foundBegin != std::string::npos && foundEnd != std::string::npos)
	{
		// Substring length
		std::size_t length = foundEnd - foundBegin;

		// Return that substring
		return source.substr(foundBegin, length);
	}

	// Didn't find anything, so return empty string
	return returnStr;
}

std::string ShaderCompiler::OutputLine(const std::string& line)
{
	return line + "\n";
}

std::string ShaderCompiler::OutputTabbedLine(const std::string& line)
{
	return "\t" + line + "\n";
}

std::string ShaderCompiler::BeginVertexHeader()
{
	std::string code = "";
	code += OutputLine("//////// VERTEX_SHADER_BEGIN ////////");
	return code;
}

std::string ShaderCompiler::OutputVertexHeader()
{
	std::string code = "";

	//Vertex Header
	code += BeginVertexHeader();

	//Version number
	code += OutputLine("#version 330 core\n");

	//Attribute layouts
	code += OutputLine("layout (location = 0) in vec3 position;");
	
	//Uniforms
	code += OutputLine("uniform mat4 u_View;");
	code += OutputLine("uniform mat4 u_Projection;");

	return code;
}

std::string ShaderCompiler::EndVertexHeader()
{
	std::string code = "";
	code += OutputLine("//////// VERTEX_SHADER_END ////////");
	return code;
}

std::string ShaderCompiler::BeginVertex()
{
	std::string code = "";
	code += "\n// Vertex Main\n";
	code += "void main()\n";
	code += "{\n";
	return code;
}

std::string ShaderCompiler::OutputVertex()
{
	//more stuff ...
	std::string code = "";

	// Final position output 
	code += OutputTabbedLine("gl_Position = u_Projection*u_View*vec4(position, 1.0);\n");
	return code;
}

std::string ShaderCompiler::EndVertex()
{
	std::string code = "";
	code += "}\n";
	code += EndVertexHeader();
	return code;
}

std::string ShaderCompiler::BeginFragmentHeader()
{
	std::string code = "";
	code += OutputLine("//////// FRAGMENT_SHADER_BEGIN ////////");
	return code;
}

std::string ShaderCompiler::OutputFragmentHeader()
{
	std::string code = "";

	// Fragment Header 
	code += BeginFragmentHeader();

	// Version number
	code += OutputLine("#version 330 core\n");

	// FragData layout
	code += OutputLine("out vec4 FragColor;");

	return code;
}

std::string ShaderCompiler::EndFragmentHeader()
{
	std::string code = "";
	code += OutputLine("//////// FRAGMENT_SHADER_END ////////");
	return code;
}

std::string ShaderCompiler::BeginFragment()
{
	std::string code = "";
	code += OutputLine("\n// Fragment Main");
	code += OutputLine("void main()");
	code += OutputLine("{");
	return code;
}

std::string ShaderCompiler::OutputFragment()
{
	//more stuff ...
	std::string code = "";
	std::string tmp_color = "vec4(1.0f, 0.0f, 0.0f, 1.0f)"; // it should be take it from shadergraph reference

	// Final position output 
	code += OutputTabbedLine("FragColor = " + tmp_color + ";\n");
	return code;
}

std::string ShaderCompiler::EndFragment()
{
	std::string code = "";
	code += "}\n";
	code += EndFragmentHeader();
	return code;
}
