#pragma once

#include "Globals.h"
#include "ResourceShader.h"
#include <list>

class ShaderGraph;
class ShaderNode;
enum NodeType;
struct Connector;

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
		

	void Generate();
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

	ShaderGraph& graph;
public:
	std::string source;
	std::string vertexCode;
	std::string fragmentCode;

};

struct ActionGraph
{
	enum ActionType
	{
		DRAG_CONNECTOR,
		RELEASE_CONNECTOR,
		DRAG_SELECTION,
		RELEASE_SELECTION,
		NONE
	};

	ActionType type = ActionType::NONE;
	Connector* connector;

	float2 mousePos;
	float2 mouseDeltaPos;

};

class ShaderGraph 
{
	friend struct ShaderCompiler;
public:
	ShaderGraph(std::string m_Name);
	~ShaderGraph();

	void Draw();
	ShaderNode* CreateNode(const char* name, int type, float2 position);

	void CompileShader(ResourceShader* shader);
	std::string GetName() const { return m_Name; }
private:
	std::string m_Name;
public:
	std::list<ShaderNode*> nodes;
	float2 scrolling;
	float scale = 1.0f;

	ShaderNode* selected = nullptr;
	ShaderNode* hovered = nullptr;

	ActionGraph action;
};