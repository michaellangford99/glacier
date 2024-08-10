#include "terrain.h"
#include <random>

terrain_geometry::terrain_geometry(std::string terrain_file, int decimation)
{
	int raw_width = TERRAIN_WIDTH_ELEMENTS;
	int raw_height = TERRAIN_HEIGHT_ELEMENTS;

	int width = raw_width / decimation;
	int height = raw_height / decimation;

	std::vector<vertex> vertices(width*height);
	std::vector<unsigned int> indices(width*height*6);

	std::ifstream input = std::ifstream(terrain_file, std::ios::binary);

	float resolution_meters_w = TERRAIN_DEGREES_PER_TILE * TERRAIN_METERS_PER_DEGREE / width;
	float resolution_meters_h = TERRAIN_DEGREES_PER_TILE * TERRAIN_METERS_PER_DEGREE / height;

	uint16_t min_height = 65535;

	float meters_per_unit = TERRAIN_METERS_PER_UNIT;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			char raw_data[2];
			input.read(raw_data, 2);
			input.seekg((decimation - 1) * 2, std::ios_base::cur);

			uint16_t height_m = (((uint16_t)raw_data[0] & 0xFF) << 8) | ((uint16_t)raw_data[1] & 0xFF);

			//std::cout << (uint16_t)raw_data[0] << "\t" << (uint16_t)raw_data[1] << "\t" << height_m << std::endl;

			if (min_height > height_m) min_height = height_m;

			vertices[i*width + j].position = { resolution_meters_w*j/ meters_per_unit,resolution_meters_h*i / meters_per_unit,height_m / meters_per_unit };
			//vertices[i*width + j].position += glm::linearRand(glm::vec3( -0.6, -0.6, 0), glm::vec3(0.6, 0.6, 0));

			//set texcoord
			vertices[i*width + j].texcoord = glm::vec2(float(j) / (float)width, (float)i / (float)height);

			//std::cout << glm::linearRand(glm::vec3(-0.2, -0.2, 0), glm::vec3(0.2, 0.2, 0)).x << std::endl;
		}

		input.seekg(2 * (raw_width%decimation), std::ios_base::cur);
		input.seekg((decimation - 1) * 2 * raw_width, std::ios_base::cur);
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//temp?
			vertices[i*width + j].position.z -= min_height / meters_per_unit;
		}
	}

	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{

			//generate normal
			glm::vec3 dx = vertices[i*width + j + 1].position - vertices[i*width + j].position;
			glm::vec3 dy = vertices[(i+1)*width + j].position - vertices[i*width + j].position;

			dx = glm::normalize(dx);
			dy = glm::normalize(dy);

			glm::vec3 normal = glm::cross(dx, dy);
			vertices[i*width + j].normal = glm::normalize(normal);
		}
	}

	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < width; j++)
		{
			indices.push_back((i - 1) * width + j - 1);
			indices.push_back((i - 0) * width + j - 0);
			indices.push_back((i - 0) * width + j - 1);

			indices.push_back((i - 1) * width + j - 1);
			indices.push_back((i - 1) * width + j - 0);
			indices.push_back((i - 0) * width + j - 0);
		}
	}

	init(vertices, indices);

	input.close();
}

//=============================================

terrain_tile::terrain_tile(std::string terrain_file, int decimation, glm::vec3 lat_long_elev)//TODO add location
{
	geometry = std::unique_ptr<terrain_geometry>(new terrain_geometry(terrain_file, decimation));
	heightmap = std::unique_ptr<texture>(new texture(terrain_file, 3601, 3601, 1, 2, GL_RED, GL_R32F, GL_SHORT, true));
	terrain_shader = std::unique_ptr<Shader>(new Shader("vertex.glsl", "terrain.glsl"));
	terrain_shader->generate_uniform_table();

	position = glm::vec3(lat_long_elev.x*TERRAIN_METERS_PER_DEGREE / TERRAIN_METERS_PER_UNIT,
						 lat_long_elev.y*TERRAIN_METERS_PER_DEGREE / TERRAIN_METERS_PER_UNIT,
						 lat_long_elev.z);
}

void terrain_tile::draw(glm::mat4x4 parent_world, Camera& camera)
{
	// FIX THIS:::
	// also, is any of this abstractable??
	// like should there be an intermediate type of 'shaded_element' that implements the matrix assignements to the shader?

	terrain_shader->use();

	//update view and pull view matrix out
	glm::mat4& view = camera.view;
	glm::mat4& projection = camera.projection;

	//set auto-edited uniforms.
	//afterwards reset those that are supposed to be set internally
	terrain_shader->set_imgui_uniforms();

	glUniformMatrix4fv(glGetUniformLocation(terrain_shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(parent_world * world));
	glUniformMatrix4fv(glGetUniformLocation(terrain_shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(terrain_shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(terrain_shader->ID, "inv_view_projection"), 1, GL_FALSE, glm::value_ptr(glm::inverse(projection * view)));
	glUniform3f(glGetUniformLocation(terrain_shader->ID, "camera_position"), camera.position.x, camera.position.y, camera.position.z);
	//glUniform1f(glGetUniformLocation(terrain_shader->ID, "eps"), shader_epsilon);
	//glUniform3f(glGetUniformLocation(terrain_shader->ID, "spos"), sphere_position.x, sphere_position.y, sphere_position.z);


	//only needs to happen once but hey whatever
	glUniform1i(glGetUniformLocation(terrain_shader->ID, "texture0"), 0);//set texture0 sampler to grab texture 0
	//glUniform1i(glGetUniformLocation(terrain_shader->ID, "texture1"), 1);//set texture1 sampler to grab texture 1

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightmap->gl_texture_id);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture1);

	geometry->draw();
}