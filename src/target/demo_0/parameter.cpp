#include "parameter.h"

/*
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
}*/

void gl_parameter::render_imgui_control(bool& value) {
    ImGui::Checkbox(name.c_str(), &value);
}

void gl_parameter::render_imgui_control(int& value) {
    ImGui::InputInt(name.c_str(), &value);
}

void gl_parameter::render_imgui_control(float& value) {
    ImGui::DragFloat(name.c_str(), &value, 0.1f);
}

void gl_parameter::render_imgui_control(glm::vec2& value) {
    ImGui::DragFloat2(name.c_str(), &value[0], 0.1f);
}

void gl_parameter::render_imgui_control(glm::vec3& value) {
    ImGui::ColorEdit3(name.c_str(), &value[0]);
}

void gl_parameter::render_imgui_control(glm::vec4& value) {
    ImGui::ColorEdit4(name.c_str(), &value[0]);
}

void gl_parameter::render_imgui_control(glm::mat3& value) {
    for (int i = 0; i < 3; i++) {
        ImGui::DragFloat3((name + " Row " + std::to_string(i)).c_str(), &value[i][0], 0.1f);
    }
}

void gl_parameter::render_imgui_control(glm::mat4& value) {
    for (int i = 0; i < 4; i++) {
        ImGui::DragFloat4((name + " Row " + std::to_string(i)).c_str(), &value[i][0], 0.1f);
    }
}

void gl_parameter::render_imgui_control(texture*& value) {
    ImGui::Text("%s: [Texture %p]", name.c_str(), value);
}

void gl_parameter::generate_imgui_editor() {
    std::visit([this](auto& val) {
        render_imgui_control(val);
    }, value);
}