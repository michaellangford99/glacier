#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

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
#include "element.h"
#include "camera.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class framebuffer
{
public:
    unsigned int ID_framebuffer;
    unsigned int width;
    unsigned int height;

    std::shared_ptr<texture> fb_texture;

    framebuffer(unsigned int width, unsigned int height, std::shared_ptr<texture> tex);
    void bind_and_predraw();
    void unbind();
    ~framebuffer();
};

#endif