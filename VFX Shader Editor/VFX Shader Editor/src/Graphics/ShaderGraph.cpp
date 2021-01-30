#include "ShaderGraph.h"

ShaderGraph::ShaderGraph()
{
}

ShaderGraph::~ShaderGraph()
{
}

ShaderCompiler::ShaderCompiler( ShaderGraph& g)
	: graph(g)
{
	code = "";
}

void ShaderCompiler::Compile()
{
	//Compile and modify the shadergraph reference
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
	code += OutputLine("//////// VERTEX_SHADER_BEGIN ////////");
	return code;
}

std::string ShaderCompiler::OutputVertexHeader()
{
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
	code += OutputLine("//////// VERTEX_SHADER_END ////////");
	return code;
}

std::string ShaderCompiler::BeginVertex()
{
	code += "\n// Vertex Main\n";
	code += "void main()\n";
	code += "{\n";
	return code;
}

std::string ShaderCompiler::OutputVertex()
{
	//more stuff ...


	// Final position output 
	code += OutputTabbedLine("gl_Position = u_Projection*u_View*vec4(position, 1.0);\n");
	return code;
}

std::string ShaderCompiler::EndVertex()
{
	code += "}\n";
	code += EndVertexHeader();
	return code;
}

std::string ShaderCompiler::BeginFragmentHeader()
{
	code += OutputLine("//////// FRAGMENT_SHADER_BEGIN ////////");
	return code;
}

std::string ShaderCompiler::OutputFragmentHeader()
{
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
	code += OutputLine("//////// FRAGMENT_SHADER_END ////////");
	return code;
}

std::string ShaderCompiler::BeginFragment()
{
	code += OutputLine("\n// Fragment Main");
	code += OutputLine("void main()");
	code += OutputLine("{");
	return code;
}

std::string ShaderCompiler::OutputFragment()
{
	//more stuff ...

	std::string tmp_color = "vec4(1.0f, 0.0f, 0.0f, 1.0f)"; // it should be take it from shadergraph reference

	// Final position output 
	code += OutputTabbedLine("FragColor = " + tmp_color + ";\n");
	return code;
}

std::string ShaderCompiler::EndFragment()
{
	code += "}\n";
	code += EndFragmentHeader();
	return code;
}
