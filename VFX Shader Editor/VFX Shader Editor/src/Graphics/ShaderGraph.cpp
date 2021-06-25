#include "ShaderGraph.h"
#include "ResourceShader.h"
#include <fstream>
//#include <sstream> 
#include <iostream>
#include <regex>
#include "ShaderNode.h"
#include "Nodes/PBRNode.h"
#include "Nodes/Parameter/VectorNode.h"
#include "Nodes/Parameter/TextureSamplerNode.h"
#include "Nodes/Math/MathBasicNode.h"
#include "Nodes/Geometry/UVNode.h"
#include "Nodes/Geometry/ParallaxOclusionNode.h"

#include "ShaderUniform.h"
#include "Texture.h"
#include "Application.h"

//=========================================================================================================================
// [GRAPH SYSTEM] SHADER GRAPH
//=========================================================================================================================
ShaderGraph::ShaderGraph(std::string m_Name)
	:m_Name(m_Name)
{
	mainNode = CreateNode("Unlit", NODE_TYPE::PBR, float2(1500.f, 350.f));
	nodes.push_back(mainNode);

	//Load default assets
	defaultTexIdx = LoadTexture2D(App, "Textures/color_white.png");
	LoadTexture2D(App, "Textures/bricks2.jpg");
	LoadTexture2D(App, "Textures/bricks2_disp.jpg");
	LoadTexture2D(App, "Textures/grass.png");
	LoadTexture2D(App, "Textures/blending.png");

	texIndices.push_back("default");
	texIndices.push_back("texture_1");
	texIndices.push_back("texture_2");
	texIndices.push_back("grass");
	texIndices.push_back("blending");

}

ShaderGraph::~ShaderGraph()
{
}


void ShaderGraph::Update(float dt)
{

	this->Input();

	this->Draw();

	this->PostUpdate(dt);

}

void ShaderGraph::Draw()
{
	
	for (std::list<ShaderLink*>::iterator it = links.begin(); it != links.end(); ++it)
	{
		//Draw stuff -----
		ImGui::PushID((*it)->UID);
		(*it)->DrawLink(*this);
		ImGui::PopID();
	}


	for (std::list<ShaderNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		ImGui::PushID((*it)->UID);

		//Draw stuff -------
		(*it)->DrawLines(*this);
		(*it)->DrawNode(*this);
		(*it)->DrawInputs(*this, (*it)->inputs.size());
		(*it)->DrawOutputs(*this, (*it)->outputs.size());

		//Update stuff -------
		(*it)->Update(*this);

		ImGui::PopID();

	}
	
}

void ShaderGraph::Input()
{
	for (std::list<ShaderLink*>::reverse_iterator it = links.rbegin(); it != links.rend(); ++it) // Interaction in reverse loop because the last one drawn shoud be the first to be interacted
	{
		//Mouse Input stuff -----
		ImGui::PushID((*it)->UID);
		(*it)->InputLink(*this);
		ImGui::PopID();
	}



	for (std::list<ShaderNode*>::reverse_iterator it = nodes.rbegin(); it != nodes.rend(); ++it) // Interaction in reverse loop because the last one drawn shoud be the first to be interacted
	{

		ImGui::PushID((*it)->UID);

		//Mouse Input stuff ------
		(*it)->InputNode(*this);
		(*it)->InputSocketInputs(*this, (*it)->inputs.size());
		(*it)->InputSocketOutputs(*this, (*it)->outputs.size());

		ImGui::PopID();
	}
}



void ShaderGraph::PostUpdate(float dt)
{


	for (std::list<ShaderNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		//Delete stuff
		if ((*it)->to_delete == true)
		{
			//delete uniform element from node
			auto uniform = uniforms.find(std::string((*it)->name) + std::to_string((*it)->UID));
			//auto uniform_depth = uniforms.find(std::string((*it)->name) + std::to_string((*it)->UID) + "_depthMap");
			
			if (uniform != uniforms.end())
			{
				RELEASE(uniform->second);
				uniforms.erase(uniform);
			}
			/*if (uniform_depth != uniforms.end())
			{
				RELEASE(uniform_depth->second);
				uniforms.erase(uniform_depth);
			}*/



			//check if node's sockets have links
			for (auto&& input : (*it)->inputs)
			{
				if (input.link_ref != nullptr)
				{
					input.link_ref->to_delete = true;
					input.link_ref->input_node = nullptr;
					//input.isLinked = false;
					//input.link_ref = nullptr;
				}
			}
			for (auto&& output : (*it)->outputs)
			{
				/*if (output.link_ref != nullptr)
				{
					output.link_ref->to_delete = true;
					output.link_ref->output_node = nullptr;
				}*/

				//output.isLinked = false;

				for (auto link : output.links_refs)
				{
					if (link != nullptr)
					{
						link->to_delete = true;
						link->output_node = nullptr;
						//link = nullptr;
					}
				}
			}

			if ((*it) == this->mainNode)
				mainNode = nullptr;

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
			if ((*it)->input_node != nullptr)
			{
				(*it)->input_node->inputs[(*it)->input_socket].isLinked = false;
				(*it)->input_node->inputs[(*it)->input_socket].link_ref = nullptr;
			}

			if ((*it)->output_node != nullptr)
			{
				(*it)->output_node->outputs[(*it)->output_socket].isLinked = false;
				//(*it)->output_node->outputs[(*it)->output_socket].link_ref = nullptr;

				for (auto link : (*it)->output_node->outputs[(*it)->output_socket].links_refs)
				{
					if(link == (*it))
						link = nullptr;
				}
			}

			RELEASE((*it));
			it = links.erase(it);
		}
	}
}

ShaderNode* ShaderGraph::CreateNode(const char* name, int type, float2 position)
{
	ShaderNode* node = nullptr;

	switch (type)
	{
	case NODE_TYPE::PBR:
		node = new PBRNode(name, (NODE_TYPE)type, position);
		this->mainNode = node;
		break;
	case NODE_TYPE::VECTOR1:
		node = new Vector1Node(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::VECTOR2:
		node = new Vector2Node(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::VECTOR3:
		node = new Vector3Node(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::VECTOR4:
		node = new Vector4Node(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::COLOR:
		node = new ColorNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::TEXTURE_SAMPLER:
		node = new TextureSamplerNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::TEXTURE:
		node = new TextureNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::UV:
		node = new UVNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::TILING_OFFSET:
		node = new TilingOffsetNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::PANNER:
		node = new PannerNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::PARALLAX_OCLUSION:
		node = new ParallaxOclusionNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::ADD:
		node = new AddNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::SUBTRACT:
		node = new SubtractNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::MULTIPLY:
		node = new MultiplyNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::DIVIDE:
		node = new DivideNode(name, (NODE_TYPE)type, position);
		break;
	case NODE_TYPE::TIME:
		node = new TimeNode(name, (NODE_TYPE)type, position);
		break;
	}

	if (node->isUniform)
	{

		switch (type)
		{
			case NODE_TYPE::TEXTURE_SAMPLER:
			{
				std::string uName = std::string(node->name) + std::to_string(node->UID);
				node->uniformLocation = this->textureSamplerLocation++;
				UniformTexture* uniform = new UniformTexture(uName, App->textures[defaultTexIdx].handle, node->uniformLocation);
				if (uniform)
				{
					this->uniforms[uniform->GetName()] = uniform;
				}

				break;
			}
			case NODE_TYPE::TIME:
			{
				std::string uName = std::string(node->name) + std::to_string(node->UID);
				node->uniformLocation = this->textureSamplerLocation++;
				UniformFloat* uniform = new UniformFloat(uName, GetTimeSinceLastCompilation(), node->uniformLocation);
				if (uniform)
				{
					this->uniforms[uniform->GetName()] = uniform;
				}

				break;
			}
			case NODE_TYPE::PARALLAX_OCLUSION:
			{
				std::string uName = std::string(node->name) + std::to_string(node->UID);
				node->uniformLocation = this->textureSamplerLocation++;
				UniformTexture* uniform = new UniformTexture(uName, App->textures[defaultTexIdx].handle, node->uniformLocation);
				if (uniform)
				{
					this->uniforms[uniform->GetName()] = uniform;
				}

				break;
			}
		
		}


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
	shader->source_code = std::string(compiler.source);
}

void ShaderGraph::ExportShader(ResourceShader* shader)
{
	ShaderCompiler compiler(*(shader->graph), shader->source_code, shader->name);
	compiler.GenerateHLSL();
}

void ShaderGraph::SetScrollOffset(float2& offset)
{
	this->scrolling = offset;
}

float ShaderGraph::GetTimeSinceLastCompilation()
{
	return (float)startup_time.ReadTime() / 1000.f;
}

void ShaderGraph::ResetNodeDeclarations()
{
	for (std::list<ShaderNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		(*it)->SetDeclared(false);
	}
}

void ShaderGraph::ResetNodeDefinitions()
{
	for (std::list<ShaderNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		(*it)->SetDefined(false);
	}
}





//=========================================================================================================================
// [SHADER COMPILER] SHADER COMPILER
//=========================================================================================================================
ShaderCompiler::ShaderCompiler( ShaderGraph& g)
	: graph(g)
{
	source = "";
}

ShaderCompiler::ShaderCompiler(ShaderGraph& g, const std::string& source, const std::string& name)
	: graph(g), glsl_source(source), shaderName(name)
{
	
}

void ShaderCompiler::GenerateHLSL()
{
	// ================  SHADERLAB DEFAULT TEMPLATE  ================

	hlsl_source = R"(Shader "Custom/ShaderName"
	{
		Properties{
			//Properties
		}

		SubShader
		{
		Tags 
		{ 
			"RenderType" = "Opaque" 
			"Queue" = "Geometry" 
		}
		
		Pass
		{
			//BlendEnable 
			//BlendEquation
		
			CGPROGRAM
		
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"
		
			//Declarations
		
		
			struct VertexInput {
				float4 vertex : POSITION;
				float2 uv:TEXCOORD0;
				float4 tangent : TANGENT;
				float3 normal : NORMAL;
			};

			struct VertexOutput {
				float4 pos : SV_POSITION;
				float2 uv:TEXCOORD0;
				float4 posWorld: TEXCOORD1;
				float3 tSpace0 : TEXCOORD2;
				float3 tSpace1 : TEXCOORD3;
				float3 tSpace2 : TEXCOORD4;
				float3 normal  : TEXCOORD5;
				
			};
		
		
			VertexOutput vert (VertexInput v)
			{
				VertexOutput o;
				o.pos = UnityObjectToClipPos (v.vertex);
				o.uv = v.uv;
                o.normal= v.normal;

				o.posWorld = mul(unity_ObjectToWorld, v.vertex);
				fixed3 worldNormal = mul(v.normal.xyz, (float3x3)unity_WorldToObject);
				fixed3 worldTangent =  normalize(mul((float3x3)unity_ObjectToWorld,v.tangent.xyz ));
				fixed3 worldBinormal = cross(worldNormal, worldTangent) * v.tangent.w; 
                o.tSpace0 = float3(worldTangent.x, worldBinormal.x, worldNormal.x);
				o.tSpace1 = float3(worldTangent.y, worldBinormal.y, worldNormal.y);
				o.tSpace2 = float3(worldTangent.z, worldBinormal.z, worldNormal.z);	

				

				return o;
			}


			fixed4 frag(VertexOutput i) : SV_Target
			{

                float3 normalDirection = normalize(i.normal);
				fixed3 worldViewDir = normalize(_WorldSpaceCameraPos.xyz - i.posWorld.xyz);
				

				//MainImage

			}

			ENDCG
		}
	  }
	Fallback "Mobile/VertexLit"
	}
	)";


	// ================  SPLIT SHADER INTO VERTEX & FRAGMENT  ================
	std::string vertexCode = ParseFromTo(BeginVertexHeader(), EndVertexHeader(), glsl_source);
	std::string fragmentCode = ParseFromTo(BeginFragmentHeader(), EndFragmentHeader(), glsl_source);


	// ================  REPLACE SHADER NAME  ================
	ReplaceString(hlsl_source, "ShaderName", shaderName);


	// ================  REPLACE BLENDING  ================
	if (graph.materialSurface == ShaderSurface::S_TRANSPARENT)
	{
		ReplaceString(hlsl_source, "//BlendEnable", "ZWrite Off");
		ReplaceString(hlsl_source, "//BlendEquation", "Blend SrcAlpha OneMinusSrcAlpha");

		ReplaceString(hlsl_source, "Opaque", "Transparent");
		ReplaceString(hlsl_source, "Geometry", "Transparent");
	}


	
	// ================  REPLACE DECLARATIONS (VARIABLES & FUNCTIONS)  ================
	int beginVars = fragmentCode.find("//////// FRAG_VARIABLES_BEGIN ////////");
	beginVars += std::string("//////// FRAG_VARIABLES_BEGIN ////////").length();
	int endVars = fragmentCode.find("//////// FRAG_VARIABLES_END ////////");
	std::size_t lengthVars = endVars - beginVars;
	std::string fragVars = fragmentCode.substr(beginVars, lengthVars);
	ReplaceString(hlsl_source, "//Declarations", fragVars);


	
	// ================  REPLACE FRAGMENT MAIN  ================
	int beginBracket = fragmentCode.find("//////// FRAG_MAIN_BEGIN ////////");
	beginBracket += std::string("//////// FRAG_MAIN_BEGIN ////////").length();
	int endBracket = fragmentCode.find("//////// FRAG_MAIN_END ////////");
	std::size_t length = endBracket - beginBracket;
	std::string fragData = fragmentCode.substr(beginBracket, length);
	ReplaceString(hlsl_source, "//MainImage", fragData);


	
	
	// ================  REPLACE PROPERTIES  ================
	std::string properties = "";
	for (std::list<ShaderNode*>::iterator it = graph.nodes.begin(); it != graph.nodes.end(); ++it)
	{
		std::string nodeName = (*it)->name + std::to_string((*it)->UID);

		if ((*it)->type == NODE_TYPE::VECTOR1)
		{
			ReplaceString(hlsl_source, (*it)->GetDefinition(), " ");
			ReplaceStringAll(hlsl_source, nodeName, "_" + nodeName);
			properties += PlacePropertyVariable(nodeName, PROPERTY_TYPES::Float);
		}
		if ((*it)->type == NODE_TYPE::VECTOR4)
		{
			ReplaceString(hlsl_source, (*it)->GetDefinition(), " ");
			ReplaceStringAll(hlsl_source, nodeName, "_" + nodeName);
			properties += PlacePropertyVariable(nodeName, PROPERTY_TYPES::Vector);
		}
		if ((*it)->type == NODE_TYPE::COLOR)
		{
			ReplaceString(hlsl_source, (*it)->GetDefinition(), " ");
			ReplaceStringAll(hlsl_source, nodeName, "_" + nodeName);
			properties += PlacePropertyVariable(nodeName, PROPERTY_TYPES::Color);
			
		}
		if ((*it)->type == NODE_TYPE::TEXTURE_SAMPLER)
		{
			ReplaceStringAll(hlsl_source, nodeName, "_" + nodeName);
			properties += PlacePropertyVariable(nodeName, PROPERTY_TYPES::Texture);
			//ReplaceString(hlsl_source, (*it)->GetDefinition(), "");
		}
		if ((*it)->type == NODE_TYPE::TIME)
		{
			ReplaceString(hlsl_source, (*it)->GetDeclaration(), "");
			ReplaceStringAll(hlsl_source, nodeName, "_Time");
		}
		if ((*it)->type == NODE_TYPE::PARALLAX_OCLUSION)
		{
			ReplaceStringAll(hlsl_source, nodeName, "_" + nodeName);
			ReplaceString(hlsl_source, "while", "[unroll(1024)]while"); //This is for posible Shader Model 2.0 error in Unity
			properties += PlacePropertyVariable(nodeName, PROPERTY_TYPES::Texture);
		}
	}

	ReplaceString(hlsl_source, "//Properties", properties);


	// ================  REPLACE TYPES  ================
	ReplaceStringAll(hlsl_source, "vec", "fixed");
	ReplaceStringAll(hlsl_source, "half", "fixed");
	ReplaceStringAll(hlsl_source, "float", "fixed");
	ReplaceStringAll(hlsl_source, "mat2", "fixed2x2");
	ReplaceStringAll(hlsl_source, "mat3", "fixed3x3");
	ReplaceStringAll(hlsl_source, "mat4", "fixed4x4");
	ReplaceStringAll(hlsl_source, "mix", "lerp");



	// ================  REPLACE ESPECIFICS  ================
	ReplaceStringAll(hlsl_source, "fs_in", "i");
	ReplaceStringAll(hlsl_source, "normalize(i.TangentViewPos - i.TangentFragPos)", "i.tSpace0.xyz * worldViewDir.x + i.tSpace1.xyz * worldViewDir.y  + i.tSpace2.xyz * worldViewDir.z");
	ReplaceStringAll(hlsl_source, "TexCoords", "uv");
	ReplaceStringAll(hlsl_source, "texture", "tex2D");
	ReplaceStringAll(hlsl_source, "FragColor =", "return");


	//Serialize to file
	WriteHLSLToFile();
}

void ShaderCompiler::WriteHLSLToFile()
{
	std::string shader_path = std::string("Shaders/ShaderLab");
	std::string fileName = shaderName + std::string(".shader");


	std::ofstream file;
	file.open((shader_path + "/" + fileName).c_str());

	if (file)
	{
		file.write(hlsl_source.c_str(), hlsl_source.length());
	}

	file.close();
}

bool ShaderCompiler::ReplaceString(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);

	return true;
}

void ShaderCompiler::ReplaceStringAll(std::string& str, const std::string& from, const std::string& to)
{
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

void ShaderCompiler::ReplaceProperty(std::string& str, const std::string& from, const std::string& to)
{
	std::string tex;
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return;
	
	
	str.replace(start_pos, from.length()+9, to);

}

std::string ShaderCompiler::PlacePropertyVariable(std::string name, PROPERTY_TYPES type)
{
	
	std::string VariableType = "";
	std::string Initialize = "";
	std::string CorrespondingVariable = "";

	switch (type) {
	case PROPERTY_TYPES::Int:
		VariableType = "Int";
		CorrespondingVariable = "int";
		Initialize = "0";
		break;
	case PROPERTY_TYPES::Float:
		VariableType = "Float";
		CorrespondingVariable = "float";
		Initialize = "0";
		break;
	case PROPERTY_TYPES::Texture:
		VariableType = "2D";
		CorrespondingVariable = "sampler2D";
		Initialize = "\"white\"{}";

			break;
	case PROPERTY_TYPES::Color:
		VariableType = "Color";
		CorrespondingVariable = "float4";
		Initialize = "(0,0,0,0)";
		break;
	case PROPERTY_TYPES::Vector:
		VariableType = "Vector";
		CorrespondingVariable = "float4";
		Initialize = "(0,0,0,1)";
		break;
	default:
		VariableType = "Int";
		CorrespondingVariable = "int";
		Initialize = "0";
		break;
	}
	

	std::string Properties = "_name (\"" + name + "\", type) = initialize";
	ReplaceString(Properties, "name", name);
	ReplaceString(Properties, "type", VariableType);
	ReplaceString(Properties, "initialize", Initialize);
	
	
	return Properties + "\n";
}

void ShaderCompiler::Generate()
{
	if (graph.mainNode == nullptr)
		return;


	//Compile and modify the shadergraph reference
	
	//Reset time
	graph.startup_time.Start();

	//Reset declarations and definitions
	graph.ResetNodeDeclarations();
	graph.ResetNodeDefinitions();

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

std::string ShaderCompiler::CheckTypeOutput(const std::string& code, const std::string& type, const std::string& requiredType)
{
	if (type.compare(requiredType) == 0)
	{
		return code;

	}
	else if (requiredType.compare("vec4") == 0)
	{
		if (type.compare("float") == 0)
		{
			return "vec4(vec3(" + code + ", " + code + ", " + code + "), 1.0 )";
		}
		else if (type.compare("vec2") == 0)
		{
			return "vec4(" + code + ", 0.0, 1.0)";
		}
		else if (type.compare("vec3") == 0)
		{
			return "vec4(" + code + ", 1.0)";
		}
		else
		{
			return code;
		}
	}
	else if (requiredType.compare("vec3") == 0)
	{
		if (type.compare("float") == 0)
		{
			return "vec3(" + code + ", " + code + ", " + code + ")";
		}
		else if (type.compare("vec2") == 0)
		{
			return "vec3(" + code + ", 1.0)";
		}
		else if (type.compare("vec4") == 0)
		{
			return code + ".xyz";
		}
		else
		{
			return code;
		}
	}
	else if (requiredType.compare("float") == 0)
	{
		if (type.compare("vec2") == 0)
		{
			return code + ".x";
		}
		else if (type.compare("vec3") == 0)
		{
			return code + ".x";
		}
		else if (type.compare("vec4") == 0)
		{
			return code + ".x";
		}
		else
		{
			return code;
		}
	}

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
	code += OutputLine("layout (location = 0) in vec3 aPos;");
	code += OutputLine("layout (location = 1) in vec3 aNormal;");
	code += OutputLine("layout (location = 2) in vec2 aTexCoords;");
	code += OutputLine("layout (location = 3) in vec3 aTangent;");
	code += OutputLine("layout (location = 4) in vec3 aBitangent;");
	
	//Uniforms
	code += OutputLine("uniform mat4 u_View;");
	code += OutputLine("uniform mat4 u_Projection;");
	code += OutputLine("uniform mat4 u_Model;");
	code += OutputLine("uniform vec3 lightPos;");
	code += OutputLine("uniform vec3 viewPos;");
	code += OutputLine("out vec3 light;");
	code += OutputLine("out vec3 view;");


	////Outs
	code += OutputLine("out VS_OUT{");
	code += OutputTabbedLine("vec3 FragPos;");
	code += OutputTabbedLine("vec2 TexCoords;");
	code += OutputTabbedLine("vec3 TangentLightPos;");
	code += OutputTabbedLine("vec3 TangentViewPos;");
	code += OutputTabbedLine("vec3 TangentFragPos;");
	code += OutputLine("} vs_out;");

	
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

	code += OutputTabbedLine("vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));\n");
	code += OutputTabbedLine("vs_out.TexCoords = aTexCoords;\n");

	code += OutputTabbedLine("vec3 T = normalize(mat3(u_Model) * aTangent);\n");
	code += OutputTabbedLine("vec3 B = normalize(mat3(u_Model) * aBitangent);\n");
	code += OutputTabbedLine("vec3 N = normalize(mat3(u_Model) * aNormal);\n");
	code += OutputTabbedLine("mat3 TBN = transpose(mat3(T, B, N));\n");

	code += OutputTabbedLine("vs_out.TangentLightPos = TBN * lightPos;\n");
	code += OutputTabbedLine("vs_out.TangentViewPos = TBN * viewPos;\n");
	code += OutputTabbedLine("vs_out.TangentFragPos = TBN * vs_out.FragPos;\n");

	
	// Final position output 
	code += OutputTabbedLine("gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);\n");

	
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

	// Ins
	code += OutputLine("in VS_OUT{\n");
	code += OutputTabbedLine("vec3 FragPos;\n");
	code += OutputTabbedLine("vec2 TexCoords;\n");
	code += OutputTabbedLine("vec3 TangentLightPos;\n");
	code += OutputTabbedLine("vec3 TangentViewPos;\n");
	code += OutputTabbedLine("vec3 TangentFragPos;\n");
	code += OutputLine("} fs_in;\n");

	// Outs
	code += OutputLine("layout(location = 0) out vec4 FragColor;\n");

	
	//Declarations
	code += OutputLine("//////// FRAG_VARIABLES_BEGIN ////////");
	InputSocket inputDiffuse = graph.mainNode->GetInputSocketbyName("Albedo");
	if (inputDiffuse.isLinked)
	{
		ShaderNode* out_node = inputDiffuse.link_ref->output_node;

		std::string varDefinition = out_node->GetOutputDeclaration(*this);
		code += varDefinition;
	}
	InputSocket inputOpacity = graph.mainNode->GetInputSocketbyName("Opacity");
	if (inputOpacity.isLinked)
	{
		ShaderNode* out_node = inputOpacity.link_ref->output_node;

		std::string varDefinition = out_node->GetOutputDeclaration(*this);
		code += varDefinition;
	}
	InputSocket inputClipAlpha = graph.mainNode->GetInputSocketbyName("Alpha Clip Threshold");
	if (inputClipAlpha.isLinked)
	{
		ShaderNode* out_node = inputClipAlpha.link_ref->output_node;

		std::string varDefinition = out_node->GetOutputDeclaration(*this);
		code += varDefinition;
	}
	code += OutputLine("//////// FRAG_VARIABLES_END ////////");

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
	std::string code = "";
	
	code += OutputLine("//////// FRAG_MAIN_BEGIN ////////");

	//Setting default variables --------
	code += OutputTabbedLine("vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);\n");
	code += OutputTabbedLine("vec2 TexCoord = fs_in.TexCoords;\n");


	//Definitions
	InputSocket inputDiffuse = graph.mainNode->GetInputSocketbyName("Albedo");
	if (inputDiffuse.isLinked)
	{
		ShaderNode* out_node = inputDiffuse.link_ref->output_node;

		std::string varDefinition = out_node->GetOutputDefinition(*this);
		code += varDefinition;


		//Final Output
		if (out_node)
		{
			//Output code variable
			std::string out_code = out_node->outputs[inputDiffuse.link_ref->output_socket].data_str;

			//Output code type 
			std::string type_code = out_node->outputs[inputDiffuse.link_ref->output_socket].type_str;

			//Check and transform typing
			out_code = CheckTypeOutput(out_code, type_code, "vec3");

			//Update Diffuse Color
			code += OutputTabbedLine("vec3 AlbedoColor = " + out_code + ";\n");
		}


	}
	else
	{
		//Set Default Diffuse Color
		 std::string tmp_color = "vec3(1.0, 1.0, 1.0)";
		 code += OutputTabbedLine("vec3 AlbedoColor = " + tmp_color + ";\n");

	}
	InputSocket inputOpacity = graph.mainNode->GetInputSocketbyName("Opacity");
	if (inputOpacity.isLinked && inputOpacity.context_type != CONTEXT_TYPE::READ_ONLY)
	{
		ShaderNode* out_node = inputOpacity.link_ref->output_node;

		std::string varDefinition = out_node->GetOutputDefinition(*this);
		code += varDefinition;

		//Final Output
		if (out_node)
		{
			//Output code variable
			std::string out_code = out_node->outputs[inputOpacity.link_ref->output_socket].data_str;

			//Output code type 
			std::string type_code = out_node->outputs[inputOpacity.link_ref->output_socket].type_str;

			//Check and transform typing
			out_code = CheckTypeOutput(out_code, type_code, "float");

			//Update Diffuse Color
			code += OutputTabbedLine("float Opacity = " + out_code +  ";\n");
		}

	}
	else
	{
		std::string tmp_color = "1.0";
		code += OutputTabbedLine("float Opacity = " + tmp_color + ";\n");
	}

	InputSocket inputAlphaClip = graph.mainNode->GetInputSocketbyName("Alpha Clip Threshold");
	if (inputAlphaClip.isLinked && inputOpacity.context_type != CONTEXT_TYPE::READ_ONLY)
	{
		ShaderNode* out_node = inputAlphaClip.link_ref->output_node;

		std::string varDefinition = out_node->GetOutputDefinition(*this);
		code += varDefinition;

		//Final Output
		if (out_node)
		{
			//Output code variable
			std::string out_code = out_node->outputs[inputAlphaClip.link_ref->output_socket].data_str;

			//Output code type 
			std::string type_code = out_node->outputs[inputAlphaClip.link_ref->output_socket].type_str;

			//Check and transform typing
			out_code = CheckTypeOutput(out_code, type_code, "float");

			//Update Diffuse Color
			code += OutputTabbedLine("float Threshold = " + out_code + ";\n");
		}
	}
	else
	{
		std::string tmp_color = "0.5";
		code += OutputTabbedLine("float Threshold = " + tmp_color + ";\n");
	}

	if (graph.clip_alpha)
	{
		code += OutputTabbedLine("if ( Opacity < Threshold ) discard;");
	}

	// Final position output 
	code += OutputTabbedLine("FragColor = vec4(AlbedoColor, Opacity);\n");

	code += OutputLine("//////// FRAG_MAIN_END ////////");
	
	return code;
}

std::string ShaderCompiler::EndFragment()
{
	std::string code = "";
	code += "}\n";
	code += EndFragmentHeader();
	return code;
}

std::string ShaderCompiler::SetOutputType(VALUE_TYPE type)
{
	std::string out_type = "";
	switch (type)
	{
		case VALUE_TYPE::FLOAT1:
			out_type = "float";
			break;
		case VALUE_TYPE::FLOAT2:
			out_type = "vec2";
			break;
		case VALUE_TYPE::FLOAT3:
			out_type = "vec3";
			break;
		case VALUE_TYPE::FLOAT4:
			out_type = "vec4";
			break;
		case VALUE_TYPE::TEXTURE2D:
			out_type = "sampler2D";
			break;

		default:
			break;
	}

	return out_type;
}






