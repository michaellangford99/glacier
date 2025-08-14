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

#include "glacier.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class demo//TODO: should this inherit glacier?
{
public:
	std::shared_ptr<glacier> g;//TODO: glacier needs to be pointer and so does active_camera in glacier.

	Camera camera = Camera(10.0f);
	std::shared_ptr<element> root;

	Shader* fullscreen_shader;
	Shader* atmosphere_shader;
	Shader* grass_shader;
	Shader* test_shader;
	std::shared_ptr<Shader> arb_function_shader;
	triangle_geometry* fullscreen_quad;
	line_geometry* quad_lines;

	std::shared_ptr<volume> test_volume;

	std::shared_ptr<texture> grass_texture;
	std::shared_ptr<texture> mask_texture;

	std::shared_ptr<terrain_tile> test_tile;
	std::shared_ptr<terrain_tile> test_tile2;
	std::shared_ptr<terrain_tile> test_tile3;
	std::shared_ptr<terrain_tile> test_tile4;

	float t = 0.0f;

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
			
			if (ImGui::TreeNode("test parameter set"))
			{
				//test_parameter_set->generate_imgui_editor();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("atmosphere_shader"))
			{
				atmosphere_shader->generate_imgui_editor();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("fullscreen_shader"))
			{
				fullscreen_shader->generate_imgui_editor();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			if (ImGui::TreeNode("grass_shader"))
			{
				grass_shader->generate_imgui_editor();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			ImGui::Separator();
			g->generate_tree_imgui_editor(root);
		}

		// Ends the window
		ImGui::End();
	}

	void add_limbs(std::vector<vertex>& vertices, glm::vec3 pos, glm::vec3 dir, float length, int depth)
	{
		vertex v = {{pos},
					{0, 0, 0},
					{0, 0},
					{1, 1, 1}};
		vertices.push_back(v);

		if (depth > 0)
		{
			glm::vec3 offset1 = glm::linearRand(glm::vec3(-0.5, -0.5, -0.3), glm::vec3(0.5, 0.5, 0.3));
			glm::vec3 offset2 = glm::linearRand(glm::vec3(-0.5, -0.5, -0.3), glm::vec3(0.5, 0.5, 0.3));
			glm::vec3 offset3 = glm::linearRand(glm::vec3(-0.5, -0.5, -0.3), glm::vec3(0.5, 0.5, 0.3));
			glm::vec3 offset4 = glm::linearRand(glm::vec3(-0.5, -0.5, -0.3), glm::vec3(0.5, 0.5, 0.3));
			add_limbs(vertices, pos+length*dir, offset1+glm::normalize(dir+glm::vec3(-0.3f,0.0f, 0.0f)), length*0.8f, depth-1);
			add_limbs(vertices, pos+length*dir, offset2+glm::normalize(dir+glm::vec3(0.6f,0.0f, 0.0f)), length*0.5f, depth-1);
			add_limbs(vertices, pos+length*dir, offset3+glm::normalize(dir+glm::vec3(0.0f,-0.2f, 0.0f)), length*0.6f, depth-1);
			add_limbs(vertices, pos+length*dir, offset4+glm::normalize(dir+glm::vec3(0.0f,0.4f, 0.0f)), length*0.7f, depth-1);
		}
	}

	demo(std::shared_ptr<glacier>  _glacier) : g(_glacier)
	{
		camera.look_at = glm::vec3(0,0,0);
		camera.update_view_projection();

		g->set_active_camera(&camera);

		//setup shaders
		atmosphere_shader = new Shader("glacier/vertex.glsl", "glacier/atmosphere_shader.glsl");
		fullscreen_shader = new Shader("glacier/vertex.glsl", "glacier/test_fullscreen_shader.glsl");
		test_shader = new Shader("glacier/vertex.glsl", "glacier/debug_fragment.glsl");
		
		//TODO fix capital/non capital letter scheme
		grass_shader = new Shader("glacier/vertex.glsl", "glacier/grass.glsl");
		grass_texture = std::shared_ptr<texture>(new texture("noise.png"));
		mask_texture = std::shared_ptr<texture>(new texture("noise_perlin.png"));

		grass_shader->set_uniform("grass_texture", grass_texture.get());
		grass_shader->set_uniform("mask_texture", mask_texture.get());

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

		test_volume = std::shared_ptr<volume>(new volume());

		std::vector<vertex> tree;
		add_limbs(tree, {0,0,0}, {0, 0, 1}, 1.0f, 10);

		quad_lines = new line_geometry(tree);

		glm::vec3 origin_lla = glm::vec3(32, -111, 2389.0f/3.0);

		test_tile  = std::make_shared<terrain_tile>("N31W111.hgt", 4, glm::vec3(31.0f,-111.0f, 0.0), origin_lla);
		test_tile2 = std::make_shared<terrain_tile>("N31W112.hgt", 4, glm::vec3(31.0f,-112.0f, 0.0), origin_lla);
		test_tile3 = std::make_shared<terrain_tile>("N32W111.hgt", 4, glm::vec3(32.0f,-111.0f, 0.0), origin_lla);
		test_tile4 = std::make_shared<terrain_tile>("N32W112.hgt", 4, glm::vec3(32.0f,-112.0f, 0.0), origin_lla);

		root = std::make_shared<element>();
		root->children.push_back(test_tile);
		root->children.push_back(test_tile2);
		root->children.push_back(test_tile3);
		root->children.push_back(test_tile4);

		for (int i = 0; i < 10; i++)
		{
			std::shared_ptr<volume> test_volume = std::shared_ptr<volume>(new volume());
			test_volume->position = glm::ballRand(30.0f);
			test_volume->position.z = abs(test_volume->position.z)/10.0;
			root->children.push_back(test_volume);
		}

		arb_function_shader = std::shared_ptr<Shader>(new Shader("glacier/vertex.glsl", "glacier/arb_function.glsl"));

	struct draw_cmd {
		glm::mat4 parent_world;
		std::shared_ptr<element> e;
	};

	std::vector<draw_cmd> translucent_draw_buffer;

	void draw_element_tree(std::shared_ptr<element>& e, glm::mat4 parent_world, Camera& camera)
	{
		e->update();
		
		if (e->premultiplied_alpha)
			translucent_draw_buffer.push_back({parent_world, e});
		else
			e->draw(parent_world, camera);

		for (auto& child : e->children)
		{
			draw_element_tree(child, parent_world*e->world, camera);
		}
	}

	void draw_all(std::shared_ptr<element>& e, glm::mat4 parent_world, Camera& camera)
	{
		translucent_draw_buffer.clear();

		//TODO: set opaque blend settings
		draw_element_tree(e, parent_world, camera);

		//set premultiplied alpha settings
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(false);//don't write to depth buffer
		//but do enable to the depth test!
		// no drawing behind stuff
		// later we will need to add functionality for translucent objects to
		// test against the depth buffer based on the illusory pixel depth, not the geomtry draw mask we use (like a cube for a circle)
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		for (auto& cmd : translucent_draw_buffer)
		{
			cmd.e->draw(cmd.parent_world, camera);
		}

		//restore? idk.
		glDepthMask(true);
	}

	void run()
	{
		std::cout << "Did I stutter" << std::endl;
		//locals:
		//...

		while(!glfwWindowShouldClose(g->window))
		{
			t += 1.0/30.0f;
			//check key and mouse input
			//processInput(window);

			//TODO: extract
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

			bool draw_fullscreen_shader = false;
			if (draw_fullscreen_shader)
			{
				//bind / apply shader?
				fullscreen_shader->bind();

				fullscreen_shader->set_imgui_uniforms();

				identity = glm::mat4(1.0);

				fullscreen_shader->set_uniform("model", identity);
				fullscreen_shader->set_uniform("view", identity);
				fullscreen_shader->set_uniform("projection", identity);
				fullscreen_shader->set_uniform("inv_view_projection", camera.inverse_view_projection);

				glDepthMask(false);
				fullscreen_quad->draw();
				glDepthMask(true);
			}

			draw_all(root, glm::mat4(1.0), camera);

			bool draw_grass_tiles = false;
			if (draw_grass_tiles)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

				grass_shader->bind();
				
				int max = 256;
				for (int i = 0; i < max; i++)
				{
					glm::mat4 world = glm::mat4(1.0);
					world = glm::translate(world, glm::vec3(0,0,20.1*(double)i/(double)max));

					glm::mat4& view = camera.view;
					glm::mat4& projection = camera.projection;

					grass_shader->set_uniform("model", world);
					grass_shader->set_uniform("view", view);
					grass_shader->set_uniform("projection", projection);
					grass_shader->set_uniform("inv_view_projection", camera.inverse_view_projection);
					grass_shader->set_uniform("camera_position", camera.position);
					grass_shader->set_uniform("t", t);

					grass_shader->set_imgui_uniforms();

					fullscreen_quad->draw();
				}
			}

			bool draw_tree = true;
			if (draw_tree)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

				test_shader->bind();
				
				//int max = 256;
				//for (int i = 0; i < max; i++)
				//{
					glm::mat4 world = glm::scale(glm::mat4(1.0), glm::vec3(0.2, 0.2, 0.2));
					//world = glm::translate(world, glm::vec3(0,0,0.1*(double)i/(double)max));

					glm::mat4& view = camera.view;
					glm::mat4& projection = camera.projection;

					test_shader->set_uniform("model", world);
					test_shader->set_uniform("view", view);
					test_shader->set_uniform("projection", projection);
					test_shader->set_uniform("debug_color", glm::vec3(1,1,1));

					test_shader->set_imgui_uniforms();

					quad_lines->draw();
				//}
			}

			//test_volume->update();
			//test_volume->draw(glm::mat4(1.0f), camera);

			//bool draw_arb_func_shader = false;
			//if (draw_arb_func_shader)
			//{
			//	glm::mat4& view = camera.view;
			//	glm::mat4& projection = camera.projection;
//
			//	arb_function_shader->bind();
			//	arb_function_shader->set_uniform("model", glm::mat4(1.0));
			//	arb_function_shader->set_uniform("view", glm::mat4(1.0));
			//	arb_function_shader->set_uniform("projection", glm::mat4(1.0));
			//	arb_function_shader->set_uniform("iTime", t);
			//	arb_function_shader->set_uniform("inv_view_projection", camera.inverse_view_projection);
			//	arb_function_shader->set_uniform("camera_position", camera.position);
//
			//	arb_function_shader->set_imgui_uniforms();
//
			//	fullscreen_quad->draw();
			//}

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

//--------------------------------
//Desired main.cpp style

/*int main()
{
	//setup functions
	//pass through mouse callbacks

	//glacier class should contain viewport shit

	//at runtime all we do is call gl commands and draw commands

	//but someone needs to manage the window parameters
	//but also be structured so this could be embedded into another window

	//imgui:
		//someone has to start, setup, and then fill in imgui editor
		//and an output of that editor is viewport parameters


	//must use all opengl crap inside scope that ends before context destruction
}*/

int main()
{
	std::shared_ptr<glacier> g = std::shared_ptr<glacier>(new glacier(800, 600));

	demo d = demo(g);

	d.run();
	
	return 0;
}