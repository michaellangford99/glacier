#include "element.h"

element::element()
{

}

void element::generate_imgui_editor()
{
    std::string sep_text = "[element],#ch=" + std::to_string(children.size());
    ImGui::SeparatorText(sep_text.c_str());

    ImGui::SliderFloat3("position", glm::value_ptr(position), -10, 10);
    ImGui::SliderFloat3("rotation", glm::value_ptr(rotation), -glm::pi<double>()*2, glm::pi<double>()*2);

}

void element::update()
{
    //update world matrix
    world = glm::rotate(glm::mat4(1.0), rotation.x, glm::vec3(0,0,1));
    world = glm::rotate(world, rotation.y, glm::vec3(1,0,0));
    world = glm::rotate(world, rotation.z, glm::vec3(0,1,0));
    world = glm::translate(world, position);
}

void element::draw(glm::mat4 parent_world, Camera& camera)
{

}