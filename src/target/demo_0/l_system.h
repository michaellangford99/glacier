#ifndef L_SYSTEM_H
#define L_SYSTEM_H

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

class l_system : public element
{
private:
	std::unique_ptr<line_geometry> lines;
	std::unique_ptr<Shader> shader;

    std::string axiom;
    std::vector<std::pair<std::string, std::string>> rules;

    std::string l;

public:
	l_system() {
        shader = std::make_unique<Shader>("glacier/vertex.glsl", "glacier/debug_fragment.glsl");
    };

    void generate()
    {
        l = axiom;

        int iter = 6;
        for (int i = 0; i < iter; i++)
        {
            std::stringstream new_l;
            for (char c : l)
            {
                bool used = false;
                for (auto& rule : rules)
                {
                    if (c == rule.first[0])
                    {
                        new_l << rule.second;
                        used = true;
                    }
                }
                if (!used) new_l << c;
            }

            l = new_l.str();
        }

        std::cout << l << std::endl;

        std::vector<vertex> tree;

        glm::vec3 pos = {0,0,0};
        float radius = 1;
        float angle = 0;
        for (char c : l)
        {
            if (c=='F' || c=='G') {
                pos += glm::vec3(0, glm::sin(angle), glm::cos(angle))*radius;
                tree.push_back(vertex(pos, {0,0,0}, {0,0}, {0,0,0}));
            }
            if (c=='-') {
                angle -= glm::radians(120.0f);
            }
            if (c=='+') {
                angle += glm::radians(120.0f);
            }
        }

        for (auto& v : tree)
            std::cout << v.position.x << "," << v.position.y << "," << v.position.z << std::endl;

        lines = std::make_unique<line_geometry>(tree);
    };

	void generate_imgui_editor(){

        element::generate_imgui_editor();

        shader->generate_imgui_editor();

        ImGui::InputText("Axiom", &axiom);

        for (auto& rule : rules)
        {
            ImGui::PushID(&rule);
            ImGui::InputText("Rule", &(rule.first));
            ImGui::InputText("Rule Generates", &(rule.second));
            ImGui::PopID();
        }

        if (ImGui::Button("Add new Rule"))
            rules.push_back({"A", "AA"});

        if (ImGui::Button("Regenerate"))
            generate();
    };

	void draw(glm::mat4 parent_world, Camera& camera) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

        shader->bind();
        
        glm::mat4 world = glm::scale(glm::mat4(1.0), glm::vec3(0.2, 0.2, 0.2));

        glm::mat4& view = camera.view;
        glm::mat4& projection = camera.projection;

        shader->set_uniform("model", world);
        shader->set_uniform("view", view);
        shader->set_uniform("projection", projection);
        shader->set_imgui_uniforms();
        if (lines)
            lines->draw();

    };
};

#endif