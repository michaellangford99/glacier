#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"
#include "triangles.h"
#include "line.h"
#include "shader.h"
#include "terrain.h"
#include "texture.h"
#include "volume.h"
#include "debug_draw.h"

#include "parameter.h"

#include "glacier.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <google/protobuf/any.h>

class demo//TODO: should this inherit glacier?
{
public:
	std::shared_ptr<glacier> g;//TODO: glacier needs to be pointer and so does active_camera in glacier.

	Camera camera = Camera(10.0f);
	std::shared_ptr<element> root;

	Shader* atmosphere_shader;
	triangle_geometry* fullscreen_quad;

	void generate_imgui_windows()
	{
		// ImGUI window creation
		ImGui::Begin("Settings");

		//TODO: add here to loop through any items that have registered to have an editor

		if (ImGui::CollapsingHeader("Camera"))
		{
			camera.generate_imgui_editor();
		}

		if (ImGui::CollapsingHeader("Shaders"))
		{
			//TODO: get all shaders on here
			

			if (ImGui::TreeNode("atmosphere_shader"))
			{
				atmosphere_shader->generate_imgui_editor();
				ImGui::TreePop();
				ImGui::Spacing();
			}
		}

		// Ends the window
		ImGui::End();
	}

	demo(std::shared_ptr<glacier>  _glacier) : g(_glacier)
	{
		camera.look_at = glm::vec3(0,0,0);
		camera.update_view_projection();

		g->set_active_camera(&camera);

		//setup shaders
		atmosphere_shader = new Shader("glacier/vertex.glsl", "glacier/atmosphere_shader.glsl");
		

		glm::vec3 plane_vertices_position[] = {
			// positions
			{1.0f,	1.0f,	0.0f},	// top right
			{1.0f,	-1.0f,	0.0f},	// bottom right
			{-1.0f,	-1.0f,	0.0f},	// bottom left
			{-1.0f,	1.0f,	0.0f}	// top left 
		};

		glm::vec2 plane_vertices_texcoord[] = {
			{1.0f, 1.0f },	// top right
			{1.0f, 0.0f},	// bottom right
			{0.0f, 0.0f},   // bottom left
			{0.0f, 1.0f}    // top left 
		};

		std::vector<vertex> plane_vertices = std::vector<vertex>(4);
		for (int i = 0; i < 4; i++)
			plane_vertices[i] = { plane_vertices_position[i],
								glm::vec3(0,1,0),
								plane_vertices_texcoord[i],
								glm::vec3(1,1,1) };

		std::vector<unsigned int> plane_indices = {
				0, 3, 1, // first triangle
				1, 3, 2  // second triangle
		};

		fullscreen_quad = new triangle_geometry(plane_vertices, plane_indices);

	}

	

	void run()
	{
		
		while(!glfwWindowShouldClose(g->window))
		{
			glViewport(g->viewport_pos.x, g->window_size.y-(g->viewport_size.y+g->viewport_pos.y), g->viewport_size.x,g->viewport_size.y);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			camera.set_viewport(g->viewport_pos, g->viewport_size, g->window_size);
			camera.update_view_projection();

			atmosphere_shader->bind();
			atmosphere_shader->set_imgui_uniforms();
			glm::mat4 identity = glm::mat4(1.0);
			atmosphere_shader->set_uniform("model", identity);
			atmosphere_shader->set_uniform("view", identity);
			atmosphere_shader->set_uniform("projection", identity);
			atmosphere_shader->set_uniform("inv_view_projection", camera.inverse_view_projection);
			atmosphere_shader->set_uniform("camera_position", camera.position);
			glDepthMask(false);
			fullscreen_quad->draw();
			glDepthMask(true);

			debug_draw::get_instance()->draw_queue(camera);
			debug_draw::get_instance()->clear_queue();

			g->imgui_init();
			generate_imgui_windows();
			g->imgui_cleanup();

			glfwSwapBuffers(g->window);//extract method probably
			glfwPollEvents();
		}
	}

};

int main()
{
	std::shared_ptr<glacier> g = std::shared_ptr<glacier>(new glacier(800, 600));

	demo d = demo(g);

	d.run();
	
	return 0;
}