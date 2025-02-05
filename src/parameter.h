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

#include <SimpleJSON/json.hpp>

class gl_parameter {
public:
	gl_parameter() {};
	gl_parameter(std::string name, bool value) : name(name), local_bool(value) { type = GL_BOOL; };
	gl_parameter(std::string name, int value) : name(name), local_int(value) { type = GL_INT; };
	gl_parameter(std::string name, float value) : name(name), local_float(value) { type = GL_FLOAT; };
	gl_parameter(std::string name, glm::vec2 value) : name(name), local_float2(value) { type = GL_FLOAT_VEC2; };
	gl_parameter(std::string name, glm::vec3 value) : name(name), local_float3(value) { type = GL_FLOAT_VEC3; };
	gl_parameter(std::string name, glm::vec4 value) : name(name), local_float4(value) { type = GL_FLOAT_VEC4; };
	gl_parameter(std::string name, glm::mat3 value) : name(name), local_mat3(value) { type = GL_FLOAT_MAT3; };
	gl_parameter(std::string name, glm::mat4 value) : name(name), local_mat4(value) { type = GL_FLOAT_MAT4; };
	gl_parameter(std::string name, texture* value) : name(name), local_texture(value) { type = GL_SAMPLER_2D; };

	gl_parameter(json::JSON serialized_value); //TODO

    std::string name;
    GLenum type;

	bool local_bool;
	int local_int;
	float local_float;
	glm::vec2 local_float2;
	glm::vec3 local_float3;
	glm::vec4 local_float4;
	glm::mat3 local_mat3;
	glm::mat4 local_mat4;
	texture* local_texture;

	float min;
	float max;

    virtual void generate_imgui_editor();
    virtual json::JSON serialize();
    virtual void write_uniform(std::shared_ptr<Shader>& shader);
};

class parameter_set {
public:
    std::map<std::string, gl_parameter> parameters;
	std::string name;

    parameter_set();

	void set_uniform(std::string name, bool value);
	void set_uniform(std::string name, int value);
	void set_uniform(std::string name, float value);
	void set_uniform(std::string name, glm::vec2 value);
	void set_uniform(std::string name, glm::vec3 value);
	void set_uniform(std::string name, glm::vec4 value);
	void set_uniform(std::string name, glm::mat3 value);
	void set_uniform(std::string name, glm::mat4 value);
	void set_uniform(std::string name, texture* value);

    void generate_imgui_editor();
    json::JSON serialize();
};

#endif