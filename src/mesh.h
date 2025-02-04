#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "triangles.h"
#include "element.h"
#include "camera.h"
#include "texture.h"
#include "framebuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class mesh : public element
{
public:
	std::unique_ptr<triangle_geometry> geometry;
	std::unique_ptr<Shader> shader;

	mesh() {};
	void generate_imgui_editor() { element::generate_imgui_editor(); };
	void draw(glm::mat4 parent_world, Camera& camera) { element::draw(parent_world, camera); };
};

#endif