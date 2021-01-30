#pragma once

#include "Globals.h"

class ShaderGraph;
struct ShaderCompiler
{
	ShaderCompiler(ShaderGraph& g);
		

	void Compile();

	//Lines
	std::string OutputLine(const std::string& line);
	std::string OutputTabbedLine(const std::string& line);

	//Vertex Shader
	std::string BeginVertexHeader();
	std::string OutputVertexHeader();
	std::string EndVertexHeader();

	std::string BeginVertex();
	std::string OutputVertex();
	std::string EndVertex();

	//Fragment Shader
	std::string BeginFragmentHeader();
	std::string OutputFragmentHeader();
	std::string EndFragmentHeader();

	std::string BeginFragment();
	std::string OutputFragment();
	std::string EndFragment();

	std::string code;
	ShaderGraph& graph;



};



class ShaderGraph 
{
public:
	ShaderGraph();
	~ShaderGraph();

private:

};