#include "parameter.h"

void parameter_set::generate_imgui_editor()
{	
	//std::string sep_text = "Shader: [vert]:'" + vertex_path + "',[frag]:'" + fragment_path + "'";

	//ImGui::SeparatorText(sep_text.c_str());

	ImGui::PushID(ID);
	if (ImGui::BeginTable("Settings", 4, ImGuiTableFlags_BordersOuter | 
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

		for (int i = 0; i < uniform_list.size(); i++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			//ImGui::PushItemWidth(ImGui::CalcItemWidth()/2);
			//ImGui::AlignTextToFramePadding();
			//ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

			std::string uniform_name = "##" + uniform_list[i].uniform_name;

			ImGui::Text(uniform_list[i].uniform_name.c_str());
			ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

			if ((uniform_list[i].uniform_name.find("color", 0) != -1) && ((uniform_list[i].uniform_type == GL_FLOAT_VEC3) || (uniform_list[i].uniform_type == GL_FLOAT_VEC4)))
			{
				//need a color picker
				if (uniform_list[i].uniform_type == GL_FLOAT_VEC3) ImGui::ColorEdit3(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float3));
				if (uniform_list[i].uniform_type == GL_FLOAT_VEC4) ImGui::ColorEdit4(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float4));
			}
			else
			{
				switch (uniform_list[i].uniform_type)
				{
				case GL_INT:   ImGui::SliderInt(uniform_name.c_str(), &uniform_list[i].local_int, uniform_list[i].min, uniform_list[i].max, "%d", ImGuiSliderFlags_AlwaysClamp); break;
				case GL_FLOAT: ImGui::SliderFloat(uniform_name.c_str(), &uniform_list[i].local_float, uniform_list[i].min, uniform_list[i].max); break;
				case GL_FLOAT_VEC2: ImGui::SliderFloat2(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float2), uniform_list[i].min, uniform_list[i].max); break;
				case GL_FLOAT_VEC3: ImGui::SliderFloat3(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float3), uniform_list[i].min, uniform_list[i].max); break;
				case GL_FLOAT_VEC4: ImGui::SliderFloat4(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float4), uniform_list[i].min, uniform_list[i].max); break;
				case GL_SAMPLER_2D:
					if (uniform_list[i].local_texture)
					{
						ImTextureID tex_id = (ImTextureID)(void*)(uniform_list[i].local_texture->gl_texture_id);
						float aspect = (float)uniform_list[i].local_texture->height/(float)uniform_list[i].local_texture->width;
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

				int min_max_width = 90;


				switch (uniform_list[i].uniform_type)
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
					ImGui::InputFloat(("##min"+std::to_string(i)).c_str(), &uniform_list[i].min, 1); 
					ImGui::TableSetColumnIndex(3);
					ImGui::SetNextItemWidth(min_max_width);
					//ImGui::SameLine();
					ImGui::InputFloat(("##max"+std::to_string(i)).c_str(), &uniform_list[i].max, 1); break;
					
					//ImGui::PopID();
				default: break;
				}
			}
		}

		ImGui::EndTable();
	}

	ImGui::PopID();
}

int parameter_set::get(std::string name)
{
	for (int i = 0; i < uniform_list.size(); i++)
	{
		if (uniform_list[i].uniform_name == name)
			return i;
	}
	//std::cout << "Could not find " << name << " in shader" << std::endl;
	return -1;
}

void parameter_set::set(std::string name, bool value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_bool = value;
}
void parameter_set::set(std::string name, int value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_int = value;
}
void parameter_set::set(std::string name, float value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_float = value;
}
void parameter_set::set(std::string name, glm::vec2 value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_float2 = value;
}
void parameter_set::set(std::string name, glm::vec3 value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_float3 = value;
}
void parameter_set::set(std::string name, glm::vec4 value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_float4 = value;
}
void parameter_set::set(std::string name, glm::mat3 value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_mat3 = value;
}
void parameter_set::set(std::string name, glm::mat4 value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_mat4 = value;
}
void parameter_set::set(std::string name, texture* value)
{
	int index = get(name);
	if (index != -1) uniform_list[index].local_texture = value;
}

template <>
void gl_parameter::render_imgui_control<bool>(const std::string& name, bool& value) {
    ImGui::Checkbox(name.c_str(), &value);
}

template <>
void gl_parameter::render_imgui_control<int>(const std::string& name, int& value) {
    ImGui::InputInt(name.c_str(), &value);
}

template <>
void gl_parameter::render_imgui_control<float>(const std::string& name, float& value) {
    ImGui::DragFloat(name.c_str(), &value, 0.1f);
}

template <>
void gl_parameter::render_imgui_control<glm::vec2>(const std::string& name, glm::vec2& value) {
    ImGui::DragFloat2(name.c_str(), &value[0], 0.1f);
}

template <>
void gl_parameter::render_imgui_control<glm::vec3>(const std::string& name, glm::vec3& value) {
    ImGui::ColorEdit3(name.c_str(), &value[0]);
}

template <>
void gl_parameter::render_imgui_control<glm::vec4>(const std::string& name, glm::vec4& value) {
    ImGui::ColorEdit4(name.c_str(), &value[0]);
}

template <>
void gl_parameter::render_imgui_control<glm::mat3>(const std::string& name, glm::mat3& value) {
    for (int i = 0; i < 3; i++) {
        ImGui::DragFloat3((name + " Row " + std::to_string(i)).c_str(), &value[i][0], 0.1f);
    }
}

template <>
void gl_parameter::render_imgui_control<glm::mat4>(const std::string& name, glm::mat4& value) {
    for (int i = 0; i < 4; i++) {
        ImGui::DragFloat4((name + " Row " + std::to_string(i)).c_str(), &value[i][0], 0.1f);
    }
}

template <>
void gl_parameter::render_imgui_control<texture*>(const std::string& name, texture*& value) {
    ImGui::Text("%s: [Texture %p]", name.c_str(), value);
}