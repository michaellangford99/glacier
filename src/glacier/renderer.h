#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

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

/*class renderer
{
    static void draw_element_tree(std::shared_ptr<element>& e, glm::mat4 parent_world, Camera& camera)
	{
		e->update();

        if (!e->premultiplied_alpha)
		    e->draw(parent_world, camera);
        else


		for (auto& child : e->children)
		{
			draw_element_tree(child, parent_world*e->world, camera);
		}
	}

    //static void set_opaque();
    //static void set_translucent();
    static void render(std::shared_ptr<framebuffer> fb,
                       std::shared_ptr<Camera> camera,
                       std::shared_ptr<element> root)
    {
        fb->bind_and_predraw();

        
        
    }

}*/

#endif