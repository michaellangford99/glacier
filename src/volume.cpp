#include "volume.h"
#include <random>

volume::volume()
{
	//geometry = std::unique_ptr<terrain_geometry>(new terrain_geometry(terrain_file, decimation));

    std::vector<vertex> cube_vertices = {
        // Front face
        {{-1, -1,  1}, {0, 0,  1}, {0, 0}, {1, 1, 1}},
        {{ 1, -1,  1}, {0, 0,  1}, {1, 0}, {1, 1, 1}},
        {{ 1,  1,  1}, {0, 0,  1}, {1, 1}, {1, 1, 1}},
        {{-1,  1,  1}, {0, 0,  1}, {0, 1}, {1, 1, 1}},
        
        // Back face
        {{ 1, -1, -1}, {0, 0, -1}, {0, 0}, {1, 1, 1}},
        {{-1, -1, -1}, {0, 0, -1}, {1, 0}, {1, 1, 1}},
        {{-1,  1, -1}, {0, 0, -1}, {1, 1}, {1, 1, 1}},
        {{ 1,  1, -1}, {0, 0, -1}, {0, 1}, {1, 1, 1}},
        
        // Left face
        {{-1, -1, -1}, {-1, 0,  0}, {0, 0}, {1, 1, 1}},
        {{-1, -1,  1}, {-1, 0,  0}, {1, 0}, {1, 1, 1}},
        {{-1,  1,  1}, {-1, 0,  0}, {1, 1}, {1, 1, 1}},
        {{-1,  1, -1}, {-1, 0,  0}, {0, 1}, {1, 1, 1}},
        
        // Right face
        {{ 1, -1,  1}, {1, 0,  0}, {0, 0}, {1, 1, 1}},
        {{ 1, -1, -1}, {1, 0,  0}, {1, 0}, {1, 1, 1}},
        {{ 1,  1, -1}, {1, 0,  0}, {1, 1}, {1, 1, 1}},
        {{ 1,  1,  1}, {1, 0,  0}, {0, 1}, {1, 1, 1}},
        
        // Top face
        {{-1,  1,  1}, {0, 1,  0}, {0, 0}, {1, 1, 1}},
        {{ 1,  1,  1}, {0, 1,  0}, {1, 0}, {1, 1, 1}},
        {{ 1,  1, -1}, {0, 1,  0}, {1, 1}, {1, 1, 1}},
        {{-1,  1, -1}, {0, 1,  0}, {0, 1}, {1, 1, 1}},
        
        // Bottom face
        {{-1, -1, -1}, {0, -1,  0}, {0, 0}, {1, 1, 1}},
        {{ 1, -1, -1}, {0, -1,  0}, {1, 0}, {1, 1, 1}},
        {{ 1, -1,  1}, {0, -1,  0}, {1, 1}, {1, 1, 1}},
        {{-1, -1,  1}, {0, -1,  0}, {0, 1}, {1, 1, 1}}
    };

    for (auto& v : cube_vertices)
        v.position /= 2;

    std::vector<unsigned int> cube_indices = {
        0, 1, 2,  2, 3, 0,   // Front
        4, 5, 6,  6, 7, 4,   // Back
        8, 9, 10, 10, 11, 8, // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20  // Bottom
    };

	geometry = std::unique_ptr<triangle_geometry>(new triangle_geometry(cube_vertices, cube_indices));

	//------ encapsulate ------

	//---- set up volume shader ----
    
	volume_shader = std::unique_ptr<Shader>(new Shader("vertex.glsl", "volume.glsl"));
	//volume_shader->set_uniform("terrain_intensity", 1.6f);
	//volume_shader->set_uniform("terrain_color", glm::vec3(0.2,0.2,0.2));
	//volume_shader->set_uniform("normal_map", normal_map.get());
	//volume_shader->set_uniform("light_dir", glm::vec3(1.0, 0.3, 1.0));
}

void volume::generate_imgui_editor()
{
	element::generate_imgui_editor();
	volume_shader->generate_imgui_editor();
}

void volume::draw(glm::mat4 parent_world, Camera& camera)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	volume_shader->bind();

	//update view and pull view matrix out
	glm::mat4& view = camera.view;
	glm::mat4& projection = camera.projection;

	//set auto-edited uniforms.
	//afterwards reset those that are supposed to be set internally

	volume_shader->set_uniform("model", parent_world * world);
	volume_shader->set_uniform("view", view);
	volume_shader->set_uniform("projection", projection);
	volume_shader->set_uniform("inv_view_projection", glm::inverse(projection * view));
	volume_shader->set_uniform("camera_position", camera.position);

	volume_shader->set_imgui_uniforms();

	geometry->draw();
}