#pragma once

#include "Globals.h"
#include "ResourceShader.h"
#include <list>
#include <unordered_map>
#include "Timer.h"

class ShaderGraph;
class ShaderNode;
enum  NODE_TYPE;
enum VALUE_TYPE;

struct ShaderLink;
class ShaderUniform;

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

	std::string CheckTypeOutput(const std::string& code, const std::string& type, const std::string& requiredType);

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

	//Build-in GLSL abstraction functions
	static std::string SetOutputType(VALUE_TYPE type);

	/*static std::string GLSLAbstractVarFloat(const std::string& value, const std::string& name);
	static std::string SetOutputVarVector2(const std::string& value1, const std::string& value2, const std::string& name);
	static std::string SetOutputVarVector3(const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& name);
	static std::string SetOutputVarVector4(const std::string& value_x, const std::string& value_y, const std::string& value_z, const std::string& value_w, const std::string& name);*/

private:

	ShaderGraph& graph;
public:
	std::string source;
	std::string vertexCode;
	std::string fragmentCode;

};



struct TempSocketData
{
	ShaderNode* node_selected = nullptr;
	unsigned int socked_selected = 0;

	bool output_socket_actived = false;
	bool input_socket_actived = false;

	float2 socket_pos;

};

struct TempNodeData
{
	ShaderNode* node_selected = nullptr;
	ShaderNode* node_hovered = nullptr;
};

class ShaderGraph 
{
	friend struct ShaderCompiler;
public:
	ShaderGraph(std::string m_Name);
	~ShaderGraph();

	void Draw();
	void Input();
	void Update(float dt);
	void PostUpdate(float dt);
	ShaderNode* CreateNode(const char* name, int type, float2 position);

	void CompileShader(ResourceShader* shader);
	std::string GetName() const { return m_Name; }

	void SetScrollOffset(float2& offset);

	float GetTimeSinceLastCompilation();
private:
	std::string m_Name;
public:
	std::list<ShaderNode*> nodes;
	std::list<ShaderLink*> links;
	float2 scrolling;
	float scale = 1.0f;

	ShaderNode* node_selected = nullptr;
	ShaderNode* node_hovered = nullptr;
	ShaderLink* link_hovered = nullptr;
	

	TempNodeData node_state;
	TempSocketData socket_state;

	ShaderNode* mainNode = nullptr;

	std::unordered_map<std::string, ShaderUniform*> uniforms;
	std::unordered_map<std::string, u32> locations;
	u32 textureSamplerLocation = 0;

	u32 defaultTexIdx;

	u32 currentTexIdx;
	std::vector<std::string> texIndices;

	Timer startup_time;
};