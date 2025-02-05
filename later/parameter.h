#ifndef PARAMETER_H
#define PARAMETER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stddef.h>
#include <map>
#include <vector>
#include <memory>
#include <variant>

#include "texture.h"

#include "shader.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class gl_parameter {
public:
    std::string name;
    std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4, texture*> value;
    GLenum uniform_type;

    void generate_imgui_editor();
    std::string serialize();
    void write_uniform(std::shared_ptr<Shader>& shader);

	template <typename T> void gl_parameter::render_imgui_control(const std::string& name, T& value) 
	{
		ImGui::Text("Unsupported type for %s", name.c_str());
	};
};

void gl_parameter::generate_imgui_editor() {
    std::visit([this](auto& val) {
        render_imgui_control(name, val);
    }, value);
}

class parameter_set {
public:
    std::map<std::string, gl_parameter> parameters;
    parameter_set();

    template <typename T>
    void set(std::string name, T value) {
        parameters[name].name = name;
        parameters[name].value = value;
    }

    void generate_imgui_editor();
    void serialize(std::string filename);
};

#endif