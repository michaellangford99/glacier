#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <map>
#include <algorithm>

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
Shader ourShader;

Shader fullscreen_shader;

GLFWwindow* create_glfw_window(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);

	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
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
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void layout_draw_imgui()
{
	// Tell OpenGL a new frame is about to begin
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// ImGUI window creation
	ImGui::Begin("My name is window, ImGUI window");
	// Text that appears in the window
	ImGui::Text("Hello there adventurer!");
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

			ourShader.generate_imgui_editor();

			ImGui::Separator();

			fullscreen_shader.generate_imgui_editor();

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
	ImGui::ShowDebugLogWindow();

	// Renders the ImGUI elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float frame_time = 0.0f;


int main()
{
	
	GLFWwindow* window = create_glfw_window(800, 600);
	if (window == NULL) return -1;

	//enable error handling
	/*int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// initialize debug output 
	}*/

	setup_camera(window);
	camera.update_view_projection();

	//setup shaders
	//ourShader = Shader("vertex.glsl", "ray_trace_viewer.glsl");
	ourShader = Shader("vertex.glsl", "terrain.glsl");
	ourShader.generate_uniform_table();

	fullscreen_shader = Shader("vertex.glsl", "test_fullscreen_shader.glsl");
	fullscreen_shader.generate_uniform_table();

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

	glm::vec3 origin_lla = glm::vec3(40.003963, -106.004053, 2419.0f);

	terrain_tile test_tile  = terrain_tile("N39W106.hgt", 2, glm::vec3(39.0f,-106.0f, 0.0), origin_lla);
	terrain_tile test_tile2 = terrain_tile("N39W107.hgt", 2, glm::vec3(39.0f,-107.0f, 0.0), origin_lla);
	terrain_tile test_tile3 = terrain_tile("N40W106.hgt", 2, glm::vec3(40.0f,-106.0f, 0.0), origin_lla);
	terrain_tile test_tile4 = terrain_tile("N40W107.hgt", 2, glm::vec3(40.0f,-107.0f, 0.0), origin_lla);

	//texture test_texture = texture("content/container.jpg");
	//setup imgui
	setup_imgui(window);

	while (!glfwWindowShouldClose(window))
	{
		//check key and mouse input
		processInput(window);
		frame_time += 1.0f / 60.0f;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		int viewport_size[4];
		glGetIntegerv(GL_VIEWPORT, viewport_size);
		camera.update_view_projection();

		test_tile.update();
		test_tile.draw(glm::mat4(1.0), camera);

		test_tile2.update();
		test_tile2.draw(glm::mat4(1.0), camera);

		test_tile3.update();
		test_tile3.draw(glm::mat4(1.0), camera);

		test_tile4.update();
		test_tile4.draw(glm::mat4(1.0), camera);

		bool draw_fullscreen_shader = false;
		if (draw_fullscreen_shader)
		{
			//bind / apply shader?
			fullscreen_shader.use();

			fullscreen_shader.set_imgui_uniforms();

			glm::mat4x4 identity = glm::mat4x4(1.0);

			glUniformMatrix4fv(glGetUniformLocation(fullscreen_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(identity));
			glUniformMatrix4fv(glGetUniformLocation(fullscreen_shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(identity));
			glUniformMatrix4fv(glGetUniformLocation(fullscreen_shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(identity));


			glUniform3f(glGetUniformLocation(fullscreen_shader.ID, "iResolution"), camera.viewport_width, camera.viewport_height, 0);
			glUniform1f(glGetUniformLocation(fullscreen_shader.ID, "iTime"), frame_time);

			fullscreen_quad.draw();
		}

		layout_draw_imgui();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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