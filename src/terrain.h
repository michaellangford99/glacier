#ifndef TERRAIN_H
#define TERRAIN_H

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

#define TERRAIN_DEGREES_PER_TILE 1.0f
#define TERRAIN_METERS_PER_DEGREE 111111.0f
#define TERRAIN_WIDTH_ELEMENTS 3601
#define TERRAIN_HEIGHT_ELEMENTS 3601
#define TERRAIN_METERS_PER_UNIT 1000.0f

class terrain_geometry : public triangle_geometry
{
public:
	terrain_geometry(std::string terrain_file, int decimation);
};

class terrain_tile : public element
{
private:
	std::unique_ptr<terrain_geometry> geometry;
	std::unique_ptr<texture> heightmap;
	std::unique_ptr<Shader> terrain_shader;
public:
	terrain_tile(std::string terrain_file, int decimation, glm::vec3 lat_long_elev);

	void draw(glm::mat4x4 parent_world, Camera& camera);
};

#endif