#include "parameter.h"

void gl_parameter::generate_imgui_editor()
{
	if ((name.find("color", 0) != -1) && ((type == GL_FLOAT_VEC3) || (type == GL_FLOAT_VEC4)))
	{
		//need a color picker
		if (type == GL_FLOAT_VEC3) ImGui::ColorEdit3(name.c_str(), glm::value_ptr(local_float3));
		if (type == GL_FLOAT_VEC4) ImGui::ColorEdit4(name.c_str(), glm::value_ptr(local_float4));
	}
	else
	{
		switch (type)
		{
		case GL_INT:   ImGui::SliderInt(name.c_str(), &local_int, min, max, "%d", ImGuiSliderFlags_AlwaysClamp); break;
		case GL_FLOAT: ImGui::SliderFloat(name.c_str(), &local_float, min, max); break;
		case GL_FLOAT_VEC2: ImGui::SliderFloat2(name.c_str(), glm::value_ptr(local_float2), min, max); break;
		case GL_FLOAT_VEC3: ImGui::SliderFloat3(name.c_str(), glm::value_ptr(local_float3), min, max); break;
		case GL_FLOAT_VEC4: ImGui::SliderFloat4(name.c_str(), glm::value_ptr(local_float4), min, max); break;
		case GL_SAMPLER_2D:
			if (local_texture)
			{
				ImTextureID tex_id = (ImTextureID)(local_texture->gl_texture_id);
				float aspect = (float)local_texture->height/(float)local_texture->width;
				//ImGui::ImageButton(("texture##"+std::to_string(i)).c_str(), tex_id, ImVec2(100, 100*aspect));
				if (ImGui::IsItemActive())
				{
					ImGui::BeginTooltip();
					int w = ImGui::GetMainViewport()->Size.x;
					int h = ImGui::GetMainViewport()->Size.y;
					ImGui::Image(tex_id, ImVec2(h*0.9f, h*0.9f*aspect));
					ImGui::EndTooltip();
				}
			}
			break;
		//TODO: add bool, matrix, textures
		
		default: break;
		}
	}
}

json::JSON gl_parameter::serialize()
{
	json::JSON obj;
	obj[name] = name;
	obj[type] = type;
	//etc.

	return obj;
}
/*json::JSON gl_parameter::serialize()
{
	switch (type)
	{
	case GL_INT:   ImGui::SliderInt(name.c_str(), &local_int, min, max, "%d", ImGuiSliderFlags_AlwaysClamp); break;
	case GL_FLOAT: ImGui::SliderFloat(name.c_str(), &local_float, min, max); break;
	case GL_FLOAT_VEC2: ImGui::SliderFloat2(name.c_str(), glm::value_ptr(local_float2), min, max); break;
	case GL_FLOAT_VEC3: ImGui::SliderFloat3(name.c_str(), glm::value_ptr(local_float3), min, max); break;
	case GL_FLOAT_VEC4: ImGui::SliderFloat4(name.c_str(), glm::value_ptr(local_float4), min, max); break;
	case GL_SAMPLER_2D:
		if (local_texture)
		{
			ImTextureID tex_id = (ImTextureID)(void*)(local_texture->gl_texture_id);
			float aspect = (float)local_texture->height/(float)local_texture->width;
			ImGui::ImageButton(("texture##"+std::to_string(i)).c_str(), tex_id, ImVec2(100, 100*aspect));
			if (ImGui::IsItemActive())
			{
				ImGui::BeginTooltip();
				int w = ImGui::GetMainViewport()->Size.x;
				int h = ImGui::GetMainViewport()->Size.y;
				ImGui::Image(tex_id, ImVec2(h*0.9f, h*0.9f*aspect));
				ImGui::EndTooltip();
			}
		}
		break;
	//TODO: add bool, matrix, textures
	
	default: break;
	}
}*/

void gl_parameter::write_uniform(std::shared_ptr<Shader>& shader)
{

}

//---------------------------------------------------------------------

void parameter_set::set_uniform(std::string name, bool value)
{
	if (parameters.count(name) != 0) parameters[name].local_bool = value;
}
void parameter_set::set_uniform(std::string name, int value)
{
	if (parameters.count(name) != 0) parameters[name].local_int = value;
}
void parameter_set::set_uniform(std::string name, float value)
{
	if (parameters.count(name) != 0) parameters[name].local_float = value;
}
void parameter_set::set_uniform(std::string name, glm::vec2 value)
{
	if (parameters.count(name) != 0) parameters[name].local_float2 = value;
}
void parameter_set::set_uniform(std::string name, glm::vec3 value)
{
	if (parameters.count(name) != 0) parameters[name].local_float3 = value;
}
void parameter_set::set_uniform(std::string name, glm::vec4 value)
{
	if (parameters.count(name) != 0) parameters[name].local_float4 = value;
}
void parameter_set::set_uniform(std::string name, glm::mat3 value)
{
	if (parameters.count(name) != 0) parameters[name].local_mat3 = value;
}
void parameter_set::set_uniform(std::string name, glm::mat4 value)
{
	if (parameters.count(name) != 0) parameters[name].local_mat4 = value;
}
void parameter_set::set_uniform(std::string name, texture* value)
{
	if (parameters.count(name) != 0) parameters[name].local_texture = value;
}

void parameter_set::generate_imgui_editor()
{	
	//std::string sep_text = "parameter_set: [vert]:'" + vertex_path + "',[frag]:'" + fragment_path + "'";

	//ImGui::SeparatorText(sep_text.c_str());

	ImGui::PushID(this);
	if (ImGui::BeginTable((name + "Settings").c_str(), 4, ImGuiTableFlags_BordersOuter | 
								 /*ImGuiTableFlags_RowBg | */
								 ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV |
								 ImGuiTableFlags_BordersOuterH |
								 ImGuiTableFlags_Resizable |
								 ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("min", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("max", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (auto pair : parameters)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			//ImGui::PushItemWidth(ImGui::CalcItemWidth()/2);
			//ImGui::AlignTextToFramePadding();
			//ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

			std::string uniform_name = "##" + pair.second.name;

			ImGui::Text("%s", pair.second.name.c_str());
			ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

			// render imgui control
			pair.second.generate_imgui_editor();

			int min_max_width = 90;
			switch (pair.second.type)
			{
			case GL_INT:
			case GL_FLOAT:
			case GL_FLOAT_VEC2:
			case GL_FLOAT_VEC3:
			case GL_FLOAT_VEC4: 

				//ImGui::PushID(i);
			
				ImGui::TableSetColumnIndex(2);
				ImGui::SetNextItemWidth(min_max_width);
				//ImGui::SameLine();
				ImGui::InputFloat(("##min"+pair.second.name).c_str(), &pair.second.min, 1); 
				ImGui::TableSetColumnIndex(3);
				ImGui::SetNextItemWidth(min_max_width);
				//ImGui::SameLine();
				ImGui::InputFloat(("##max"+pair.second.name).c_str(), &pair.second.max, 1); break;
				
				//ImGui::PopID();
			default: break;
			}
		}

		ImGui::EndTable();
	}

	ImGui::PopID();
}

json::JSON parameter_set::serialize()
{
	json::JSON obj;
	for (auto pair : parameters){
		obj[pair.second.name] = pair.second.serialize();
	}

	return obj;
}