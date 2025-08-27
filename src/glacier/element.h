#ifndef ELEMENT_H
#define ELEMENT_H

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

#include "triangles.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class element
{
public:
	std::vector<std::shared_ptr<element>> children;

	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 rotation = glm::vec3(0,0,0);
	
	glm::mat4 world;

	//temp, indicate which draw system we should use
	bool premultiplied_alpha = false;
	bool needs_depth_map = false;

	element();
	virtual void generate_imgui_editor();
	virtual void update();
	virtual void draw(glm::mat4 parent_world, Camera& camera);
};

#endif