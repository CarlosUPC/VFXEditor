#pragma once

#include "Globals.h"

class ShaderGraph;

enum class ShaderType
{
	VERTEX,
	FRAGMENT,
	UNKNOWN
};

struct ShaderCompiler
{

public:
	ShaderCompiler(ShaderGraph& g);
		

	void Compile();
	void WriteShaderToFile();
	std::string SplitShaderSource(ShaderType type);
	std::string ParseFromTo(const std::string& begin, const std::string& end, const std::string& source);
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


private:

	std::string code;
	ShaderGraph& graph;



};



class ShaderGraph 
{
	friend struct ShaderCompiler;
public:
	ShaderGraph();
	~ShaderGraph();

private:
	std::string m_Name;

};