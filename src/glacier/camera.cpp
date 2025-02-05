#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(float _radius)
{
	radius = _radius;
}

void Camera::generate_imgui_editor()
{
	ImGui::SliderFloat("mouse_sensitivity", &(mouse_sensitivity), 0.001f, 0.01f);
	ImGui::SliderFloat("scroll_sensitivity", &(scroll_sensitivity), 0.2f, 2.0f);

	ImGui::SliderFloat("near_plane", &near_plane, 0.01f, 1.0f);
	ImGui::SliderFloat("far_plane", &far_plane, 50.0f, 300.0f);
}

void Camera::update_view_projection()
{
	float camZ = sin(pitch) * radius;
	float proj_radius = cos(pitch) * radius;
	float camX = cos(yaw) * proj_radius;
	float camY = sin(yaw) * proj_radius;
	position = glm::vec3(camX, camY, camZ) + look_at;
	view = glm::lookAt(position, look_at, glm::vec3(0.0, 0.0, 1.0));

	int viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);

	if (viewport_size[2] != 0 && viewport_size[3] != 0)
	{
		viewport_width = viewport_size[2];
		viewport_height = viewport_size[3];
	}

	projection = glm::perspective(glm::radians(45.0f), (float)viewport_width / (float)viewport_height, near_plane, far_plane);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//TODO: camera needs reference to its window and needs to bounds check this callback, not just for Imgui
	//but also remember that maybe ImGui is holding a container that is using the camera
	//Not sure how to handle stacks of windows holding a camera, as all the callbacks will get called.

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	xoffset *= mouse_sensitivity;
	yoffset *= mouse_sensitivity;

	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		if (mouse_down)
		{
			yaw -= xoffset;
			pitch -= yoffset;

			if (pitch > 89.0f*glm::pi<float>() / 180.0f)
				pitch = 89.0f*glm::pi<float>() / 180.0f;
			if (pitch < -89.0f*glm::pi<float>() / 180.0f)
				pitch = -89.0f*glm::pi<float>() / 180.0f;
		}
	}
}

void Camera::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		mouse_down = true;
	else
		mouse_down = false;
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		radius -= scroll_sensitivity*yoffset;
	}
}