#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#define IMGUI_USER_CONFIG
#define IMGUI_ENABLE_FREETYPE

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"
#include "triangles.h"
#include "shader.h"
#include "terrain.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

//All globals

Camera camera(10.0f);

Shader* fullscreen_shader;
Shader* grass_shader;

std::shared_ptr<element> root;

//viewport parameters
glm::vec2 viewport_pos = {0,0};
glm::vec2 viewport_size;
glm::vec2 window_size;

void set_render_area(glm::vec2 pos, glm::vec2 size);

GLFWwindow* create_glfw_window(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(width, height, "Glacier", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, width, height);
	//glEnable(GL_MULTISAMPLE);
	return window;
}

void setup_camera(GLFWwindow* window)
{
	//setup camera and camera control
	camera = Camera(20.0f);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void setup_imgui(GLFWwindow* window)
{
	//
	//Initialize ImGUI
	//
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImFontConfig imf = ImFontConfig();
	imf.OversampleH = 3;
	imf.OversampleV = 3;
	imf.PixelSnapH = true;
	//io.Fonts->AddFontFromFileTTF("imgui/misc/fonts/DroidSans.ttf", 14.5);
	io.Fonts->AddFontFromFileTTF("Ubuntu-R.ttf", 13);

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.FrameRounding = 2.0f;
	style.WindowRounding = 3.0f;
	style.ChildRounding = 2.0f;
	style.GrabRounding = 2.0f;
	style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
	style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void recurse_imgui_tree(std::shared_ptr<element> e)
{
	e->generate_imgui_editor();

	int c_idx = 0;
	for (auto &c : e->children)
	{
		if (ImGui::TreeNode(("element "+std::to_string(c_idx++)).c_str()))
		{
			recurse_imgui_tree(c);

			ImGui::TreePop();
			ImGui::Spacing();
		}
	}
}

void generate_tree_imgui_editor()
{
	if (root)
	{
		recurse_imgui_tree(root);
	}
}

void layout_draw_imgui()
{
	// Tell OpenGL a new frame is about to begin
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiID id = ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode, nullptr);
	ImGuiDockNode* node = ImGui::DockBuilderGetCentralNode(id);

	// ImGUI window creation
	ImGui::Begin("Settings");
	// Checkbox that appears in the window
	//ImGui::Checkbox("Draw Triangle", &drawTriangle);
	// Slider that appears in the window
	//ImGui::SliderFloat("greenValue", &greenValue, 0.0f, 1.0f);
	//ImGui::SliderFloat("radius", &radius, 1.0f, 20.0f);

	//ImGui::SliderFloat("mouse_sensitivity", &(camera.mouse_sensitivity), 0.001f, 0.01f);
	//ImGui::SliderFloat("scroll_sensitivity", &(camera.scroll_sensitivity), 0.2f, 2.0f);

	//ImGui::SliderFloat3("position", glm::value_ptr(position), -2, 2);

	/*ImGui::BeginMenuBar();
	ImGui::BeginMenu("test menu", true);
	//ImGui::SliderAngle("angle", &test_angle, -180.0f, 180.0f);
	ImGui::EndMenu();
	ImGui::EndMenuBar();*/

	

	//ImGui::CollapsingHeader("Layout & Scrolling");
	//if (ImGui::CollapsingHeader("Layout & Scrolling"))
	//{
	//	if (ImGui::TreeNode("Child windows"))
	//	{

	//TODO: add here to loop through any items that have registered to have an editor

	if (ImGui::CollapsingHeader("Camera"))
	{
		camera.generate_imgui_editor();
	}

	if (ImGui::CollapsingHeader("Shaders"))
    {
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
		generate_tree_imgui_editor();
	}


	//		ImGui::TreePop();
	//	}
	//}
	//ImGui::TreeNode("Child windows");
	//ImGui::TreeNode("Child windows");
	//ImGui::TreePop();


	// Fancy color editor that appears in the window
	//ImGui::ColorEdit4("sunrise_color", sunrise_color);
	//ImGui::ColorEdit4("sunset_color", sunset_color);
	// Ends the window
	ImGui::End();

	ImGui::ShowDemoWindow();
	//ImGui::ShowDebugLogWindow();

	set_render_area({node->Pos.x, node->Pos.y}, {node->Size.x, node->Size.y});

	// Renders the ImGUI elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
	window_size = {800, 600};

	GLFWwindow* window = create_glfw_window(window_size.x, window_size.y);
	if (window == NULL) return -1;

	setup_camera(window);

	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	viewport_pos = {vp[0], vp[1]};
	viewport_size = {vp[2], vp[3]};
	window_size = viewport_size;

	camera.update_view_projection();

	camera.look_at = glm::vec3(0,0.1,0);

	//setup shaders

	fullscreen_shader = new Shader("vertex.glsl", "test_fullscreen_shader.glsl");
	
	//TODO fix capital/non capital letter scheme
	grass_shader = new Shader("vertex.glsl", "grass.glsl");
	texture grass_texture = texture("noise.png");
	texture mask_texture = texture("noise_perlin.png");

	grass_shader->set_uniform("grass_texture", &grass_texture);
	grass_shader->set_uniform("mask_texture", &mask_texture);

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

	triangle_geometry fullscreen_quad = triangle_geometry(plane_vertices, plane_indices);

	glm::vec3 origin_lla = glm::vec3(32, -111, 2389.0f/3.0);

	std::shared_ptr<terrain_tile> test_tile  = std::make_shared<terrain_tile>("N31W111.hgt", 20, glm::vec3(31.0f,-111.0f, 0.0), origin_lla);
	std::shared_ptr<terrain_tile> test_tile2 = std::make_shared<terrain_tile>("N31W112.hgt", 20, glm::vec3(31.0f,-112.0f, 0.0), origin_lla);
	std::shared_ptr<terrain_tile> test_tile3 = std::make_shared<terrain_tile>("N32W111.hgt", 20, glm::vec3(32.0f,-111.0f, 0.0), origin_lla);
	std::shared_ptr<terrain_tile> test_tile4 = std::make_shared<terrain_tile>("N32W112.hgt", 20, glm::vec3(32.0f,-112.0f, 0.0), origin_lla);

	root = std::make_shared<element>();
	root->children.push_back(test_tile);
	root->children.push_back(test_tile2);
	root->children.push_back(test_tile3);
	root->children.push_back(test_tile4);

	std::shared_ptr<Shader> arb_function_shader = std::shared_ptr<Shader>(new Shader("vertex.glsl", "arb_function.glsl"));

	//texture test_texture = texture("content/container.jpg");
	//setup imgui
	setup_imgui(window);
	float t=0.0f;
	while (!glfwWindowShouldClose(window))
	{
		t += 1.0/30.0f;
		//check key and mouse input
		processInput(window);
		//frame_time += 1.0f / 60.0f;

		glViewport(viewport_pos.x, window_size.y-(viewport_size.y+viewport_pos.y), viewport_size.x,viewport_size.y);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		camera.update_view_projection();

		bool draw_terrain = false;
		if (draw_terrain)
		{
			test_tile->update();
			test_tile->draw(glm::mat4(1.0), camera);

			test_tile2->update();
			test_tile2->draw(glm::mat4(1.0), camera);

			test_tile3->update();
			test_tile3->draw(glm::mat4(1.0), camera);

			test_tile4->update();
			test_tile4->draw(glm::mat4(1.0), camera);
		}

		bool draw_grass_tiles = true;
		if (draw_grass_tiles)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

			grass_shader->bind();
			
			int max = 256;
			for (int i = 0; i < max; i++)
			{
				glm::mat4 world = glm::mat4(1.0);
				world = glm::translate(world, glm::vec3(0,0,0.1*(double)i/(double)max));

				glm::mat4& view = camera.view;
				glm::mat4& projection = camera.projection;

				grass_shader->set_uniform("model", world);
				grass_shader->set_uniform("view", view);
				grass_shader->set_uniform("projection", projection);
				grass_shader->set_uniform("inv_view_projection", glm::inverse(projection * view));
				grass_shader->set_uniform("camera_position", camera.position);
				grass_shader->set_uniform("t", t);

				grass_shader->set_imgui_uniforms();

				fullscreen_quad.draw();
			}
		}

		bool draw_fullscreen_shader = false;
		if (draw_fullscreen_shader)
		{
			//bind / apply shader?
			fullscreen_shader->bind();

			fullscreen_shader->set_imgui_uniforms();

			glm::mat4 identity = glm::mat4(1.0);

			glUniformMatrix4fv(glGetUniformLocation(fullscreen_shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(identity));
			glUniformMatrix4fv(glGetUniformLocation(fullscreen_shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(identity));
			glUniformMatrix4fv(glGetUniformLocation(fullscreen_shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(identity));

			glUniform3f(glGetUniformLocation(fullscreen_shader->ID, "iResolution"), camera.viewport_width, camera.viewport_height, 0);
			//glUniform1f(glGetUniformLocation(fullscreen_shader->ID, "iTime"), frame_time);

			fullscreen_quad.draw();
		}

		bool draw_arb_func_shader = false;
		if (draw_arb_func_shader)
		{
			glm::mat4& view = camera.view;
			glm::mat4& projection = camera.projection;

			arb_function_shader->bind();
			arb_function_shader->set_uniform("model", glm::mat4(1.0));
			arb_function_shader->set_uniform("view", glm::mat4(1.0));
			arb_function_shader->set_uniform("projection", glm::mat4(1.0));
			arb_function_shader->set_uniform("iTime", t);
			arb_function_shader->set_uniform("inv_view_projection", glm::inverse(projection * view));
			arb_function_shader->set_uniform("camera_position", camera.position);

			arb_function_shader->set_imgui_uniforms();

			fullscreen_quad.draw();
		}

		layout_draw_imgui();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete fullscreen_shader;

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void set_render_area(glm::vec2 pos, glm::vec2 size)
{
	viewport_pos = pos;
	viewport_size = size;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_size = {width, height};//???
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	camera.mouse_button_callback(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.scroll_callback(window, xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}