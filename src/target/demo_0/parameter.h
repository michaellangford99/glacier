#ifndef PARAMETER_H
#define PARAMETER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stddef.h>
#include <map>
#include <vector>
#include <memory>
#include <variant>

#include "texture.h"

#include "shader.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "element.h"
#include "framebuffer.h"
#include "debug_draw.h"

class gl_parameter {
public:
    std::string name;
    std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4, texture*> value;
    GLenum uniform_type;

    void generate_imgui_editor();
    std::string serialize();
    void write_uniform(std::shared_ptr<Shader>& shader);

	virtual void render_imgui_control(bool& value);
    virtual void render_imgui_control(int& value);
    virtual void render_imgui_control(float& value);
    virtual void render_imgui_control(glm::vec2& value);
    virtual void render_imgui_control(glm::vec3& value);
    virtual void render_imgui_control(glm::vec4& value);
    virtual void render_imgui_control(glm::mat3& value);
    virtual void render_imgui_control(glm::mat4& value);
    virtual void render_imgui_control(texture*& value);
};

class different_vec3_editor : public gl_parameter
{
public:
    std::shared_ptr<texture> fb_tex;
    std::shared_ptr<framebuffer> fb;

    Camera camera;

    different_vec3_editor()
    {
        fb_tex = std::make_shared<texture>(nullptr, 300, 300, 4, 2, GL_RGBA, GL_RGBA, GL_SHORT);
        fb = std::make_shared<framebuffer>(300, 300, fb_tex);

        camera = Camera(10.0f);
        camera.set_pitch(glm::radians(45.0f));
        camera.set_yaw(glm::radians(45.0f));
        camera.set_viewport({0,0}, {300, 300}, {300, 300});
        camera.update_view_projection();
    }

    void render_imgui_control(glm::vec3& value)
    {
        fb->bind_and_predraw();

        debug_draw_request ddr_basis;
        ddr_basis.color = glm::vec3(1,1,1);
        ddr_basis.lines = debug_draw::get_instance()->basis;
        ddr_basis.world = glm::mat4(1.0);//element::create_rotation(glm::mat4(1.0), value);//glm::translate(glm::mat4(1.0), value);
        debug_draw::get_instance()->draw_immediate(camera, ddr_basis);
        
        debug_draw_request ddr;
        ddr.color = glm::vec3(0,1,0);
        ddr.lines = debug_draw::get_instance()->ones_line;
        ddr.world = glm::scale(glm::mat4(1.0), value);//element::create_rotation(glm::mat4(1.0), value);//glm::translate(glm::mat4(1.0), value);
        debug_draw::get_instance()->draw_immediate(camera, ddr);

        fb->unbind();

        ImGui::SliderFloat3(name.c_str(), &value[0], -1.0f, 1.0f);

        unsigned int w = fb_tex->width;
        unsigned int h = fb_tex->height;
        unsigned int gl_tex_id = fb_tex->gl_texture_id;

        ImTextureID tex_id = (ImTextureID)(void*)(gl_tex_id);
        //float aspect = (float)h/(float)w;
        ImGui::ImageButton("texture", tex_id, ImVec2(w, h), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		

    }
};

#endif