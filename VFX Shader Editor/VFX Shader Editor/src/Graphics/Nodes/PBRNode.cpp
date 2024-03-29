#include "PBRNode.h"
#include "ShaderGraph.h"

//=========================================================================================================================
// [NODE] UNLIT NODE
//=========================================================================================================================
PBRNode::PBRNode()
{
}

PBRNode::PBRNode(const char* name, NODE_TYPE type, float2 position)
	: ShaderNode(name, type, position)
{
	inputs.push_back(InputSocket("Albedo", VALUE_TYPE::FLOAT3));
	inputs.push_back(InputSocket("Alpha Clip Threshold", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::READ_ONLY));
	inputs.push_back(InputSocket("Opacity", VALUE_TYPE::FLOAT1, CONTEXT_TYPE::READ_ONLY));
	
	//temp hardcoded
	inputs_size = 3;
	outputs_size = 1;
}

void PBRNode::Update(ShaderGraph& g)
{
}

void PBRNode::InspectorUpdate(ShaderGraph& graph)
{
	if (ImGui::CollapsingHeader("Node Configuration", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
		ImGui::Text("Shader Surface");
		const char* shader_surface_list[] = { "Opaque", "Transparent" };
		static int item_surface = (int)graph.materialSurface;
		if (ImGui::Combo("##surface_mode", &item_surface, shader_surface_list, IM_ARRAYSIZE(shader_surface_list)))
		{
			graph.materialSurface = (ShaderSurface)item_surface;

			if (graph.clip_alpha)
			{
				if (graph.materialSurface == ShaderSurface::S_OPAQUE)
				{
					inputs[1].context_type = CONTEXT_TYPE::NONE;
					inputs[2].context_type = CONTEXT_TYPE::NONE;
					inputs_size = 3;
				}
				else if (graph.materialSurface == ShaderSurface::S_TRANSPARENT)
				{
					inputs[1].context_type = CONTEXT_TYPE::NONE;
					inputs[2].context_type = CONTEXT_TYPE::NONE;
					inputs_size = 3;
				}
			}
			else
			{
				if (graph.materialSurface == ShaderSurface::S_OPAQUE)
				{
					inputs[2].context_type = CONTEXT_TYPE::READ_ONLY;
					inputs[1].context_type = CONTEXT_TYPE::READ_ONLY;
					inputs_size = 3;
				}
				else if (graph.materialSurface == ShaderSurface::S_TRANSPARENT)
				{
					inputs[2].context_type = CONTEXT_TYPE::NONE;
					inputs[1].context_type = CONTEXT_TYPE::READ_ONLY;
					inputs_size = 3;
				}
			}
		}

		

		ImGui::Separator();

		ImGui::Text("Alpha Clipping");
		ImGui::SameLine();
		if (ImGui::Checkbox("##clipalpha", &graph.clip_alpha))
		{
			if (graph.clip_alpha)
			{
				if (graph.materialSurface == ShaderSurface::S_OPAQUE)
				{
					inputs[1].context_type = CONTEXT_TYPE::NONE;
					inputs[2].context_type = CONTEXT_TYPE::NONE;
					inputs_size = 3;
				}
				else if (graph.materialSurface == ShaderSurface::S_TRANSPARENT)
				{
					inputs[1].context_type = CONTEXT_TYPE::NONE;
					inputs[2].context_type = CONTEXT_TYPE::NONE;
					inputs_size = 3;
				}
			}
			else
			{
				if (graph.materialSurface == ShaderSurface::S_OPAQUE)
				{
					inputs[2].context_type = CONTEXT_TYPE::READ_ONLY;
					inputs[1].context_type = CONTEXT_TYPE::READ_ONLY;
					inputs_size = 3;
				}
				else if (graph.materialSurface == ShaderSurface::S_TRANSPARENT)
				{
					inputs[2].context_type = CONTEXT_TYPE::NONE;
					inputs[1].context_type = CONTEXT_TYPE::READ_ONLY;
					inputs_size = 3;
				}
			}

		}
	
	}

}