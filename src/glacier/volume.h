#ifndef VOLUME_H
#define VOLUME_H

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
#include "mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class volume : public mesh
{
public:
    std::shared_ptr<Shader> volume_shader;
	volume(std::string vertex_shader="glacier/vertex.glsl", 
		   std::string fragment_shader="glacier/volume.glsl");
	void generate_imgui_editor();
	void draw(glm::mat4 parent_world, Camera& camera);
};

#endif