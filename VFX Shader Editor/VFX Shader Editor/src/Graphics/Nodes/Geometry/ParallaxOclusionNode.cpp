#include "ParallaxOclusionNode.h"
#include "ShaderGraph.h"

ParallaxOclusionNode::ParallaxOclusionNode()
{
}

ParallaxOclusionNode::ParallaxOclusionNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{

	inputs.push_back(InputSocket("UV", VALUE_TYPE::FLOAT2));
	inputs.push_back(InputSocket("Amplitude", VALUE_TYPE::FLOAT1, 0.01f, CONTEXT_TYPE::NONE));
	inputs.push_back(InputSocket("Min Steps", VALUE_TYPE::FLOAT1, 8.0f, CONTEXT_TYPE::NONE));
	inputs.push_back(InputSocket("Max Steps", VALUE_TYPE::FLOAT1, 32.0f, CONTEXT_TYPE::NONE));
	inputs.push_back(InputSocket("Heightmap", VALUE_TYPE::TEXTURE2D));

	outputs.push_back(OutputSocket("ParallaxUV", VALUE_TYPE::FLOAT2));

	//temp hardcoded
	inputs_size = 8;
	outputs_size = 1;

	isUniform = true;



}

void ParallaxOclusionNode::Update(ShaderGraph& graph)
{
	//Outs
	outputs[0].data_str = "ParallaxOclusion" + std::to_string(UID);
	outputs[0].type_str = ShaderCompiler::SetOutputType(outputs[0].type);

	//Ins
	inputs[0].data_str = "TexCoord";
	inputs[1].data_str = std::to_string(inputs[1].value1);
	inputs[2].data_str = std::to_string(inputs[2].value1);
	inputs[3].data_str = std::to_string(inputs[3].value1);
	//inputs[4].data_str = name + std::to_string(UID) + "_depthMap";

	//Check For Ins Variables
	CheckNodeConnections(this, graph);

	//GLSL Abstraction
	this->GLSL_Declaration = SetGLSLDeclaration(outputs[0].data_str);
	this->GLSL_Definition = SetGLSLDefinition(outputs[0].data_str, inputs[0].data_str, inputs[1].data_str, inputs[2].data_str, inputs[3].data_str);

}

void ParallaxOclusionNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//Amplitude ------------
		if (!inputs[1].isLinked)
		{
			ImGui::PushID("##X");
			ImGui::Text("Height Scale "); ImGui::SameLine();
			ImGui::DragFloat("##amplitude", &inputs[1].value1, 0.01f, 0.f, 1.0f, "%.3f");
			ImGui::PopID();			
		}

		//Min Layers ------------
		if (!inputs[2].isLinked)
		{
			ImGui::PushID("##X");
			ImGui::Text("Min Layers "); ImGui::SameLine();
			ImGui::DragInt("##minl", (int*)&inputs[2].value1, 1.0f, 0, 64, "%.1f");
			ImGui::PopID();
		}

		//Max Layers ------------
		if (!inputs[3].isLinked)
		{
			ImGui::PushID("##X");
			ImGui::Text("Max Layers "); ImGui::SameLine();
			ImGui::DragInt("##maxl", (int*)&inputs[3].value1, 1.0f, 0, 64, "%.1f");
			ImGui::PopID();
		}

		//Heightmap Texture ------------
		if (!inputs[4].isLinked)
		{
			
		}


	}

	if (ImGui::CollapsingHeader("GLSL Abstraction", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(GLSL_Definition.c_str());
	}

}

std::string ParallaxOclusionNode::SetGLSLDeclaration(const std::string& out_name)
{
	return	"uniform sampler2D " + out_name + "_depthMap;\n"
			"vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, float heightScale, int minLayers, int maxLayers)\n"
			"{\n"
			"    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));\n"
			"    float layerDepth = 1.0 / numLayers;\n"
			"    float currentLayerDepth = 0.0;\n"
			"    vec2 P = viewDir.xy / viewDir.z * heightScale;\n"
			"    vec2 deltaTexCoords = P / numLayers;\n"
			"    vec2  currentTexCoords = texCoords;\n"
			"    float currentDepthMapValue = texture(" + out_name + "_depthMap, currentTexCoords).r;\n"
			"\n"
			"    while (currentLayerDepth < currentDepthMapValue)\n"
			"    {\n"
			"		currentTexCoords -= deltaTexCoords;\n"
			"		currentDepthMapValue = texture(" + out_name + "_depthMap, currentTexCoords).r;\n"
			"		currentLayerDepth += layerDepth;\n"
			"    }\n"
			"\n"
			"    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;\n"
			"    float afterDepth = currentDepthMapValue - currentLayerDepth;\n"
			"    float beforeDepth = texture(" + out_name + "_depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;\n"
			"    float weight = afterDepth / (afterDepth - beforeDepth);\n"
			"    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);\n"
			"    return finalTexCoords;\n"
			"}\n";
}

std::string ParallaxOclusionNode::SetGLSLDefinition(const std::string& out_name, const std::string& uv,  const std::string& amplitude, const std::string& min_layers, const std::string& max_layers)
{
	return "vec2 " + out_name + " = ParallaxMapping(" + uv + ", " + "viewDir" + ", " + amplitude + ", " +  "8" + ", " +  "32" + ");\n";
}
