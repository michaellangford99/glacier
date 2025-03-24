#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

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

#include "parameter.h"

class parameter_set {
public:
    std::map<std::string, std::unique_ptr<gl_parameter>> parameters;
    parameter_set() {};

    template <typename T>
    void set(std::string name, T value) {
        if (!parameters.count(name))
            parameters[name] = std::unique_ptr<gl_parameter>(new gl_parameter());
        (parameters[name])->name = name;
        (parameters[name])->value = value;
    }
    
    void generate_imgui_editor();
    void serialize(std::string filename);
};

#endif