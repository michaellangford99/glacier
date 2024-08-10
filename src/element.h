#ifndef ELEMENT_H
#define ELEMENT_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "triangles.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class element
{
public:
	std::vector<std::shared_ptr<element>> children;

	glm::vec3 position;
	glm::vec3 rotation;
	
	glm::mat4x4 world;

	element();
	virtual void update();
	virtual void draw(glm::mat4x4 parent_world, Camera& camera);
};

#endif