#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <vector>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

#include "triangles.h"
#include "line.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct debug_draw_request {
    glm::mat4 world;
    glm::vec3 color;
    std::shared_ptr<line_geometry> lines;
};

struct debug_draw_str_request {
    glm::vec2 vp_pos;
    glm::vec4 color;
    std::string text;
};

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class debug_draw {
private:
    static debug_draw* instance;

    std::shared_ptr<line_geometry> line;
    std::shared_ptr<line_geometry> basis;
    std::shared_ptr<Shader> debug_shader;

    std::vector<debug_draw_request> debug_draw_queue;
    std::vector<debug_draw_str_request> debug_draw_str_queue;

    debug_draw();
public:
    debug_draw(const debug_draw&) = delete;
    debug_draw& operator=(const debug_draw&) = delete;

    // Get instance of singleton
    static debug_draw* get_instance() {
        if (!instance) {
            instance = new debug_draw();
        }
        return instance;
    }

    void draw_line(glm::vec3 start, glm::vec3 end, glm::vec3 color);
    void draw_basis(glm::mat4 world, glm::vec3 color);

    // text rendering stuff:
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
    std::unique_ptr<Shader> text_shader;
    void draw_string(std::string text, glm::vec3 world_pos, glm::vec4 color, Camera& camera);
    void draw_string(std::string text, glm::vec2 pos_vp, glm::vec4 color);
    void draw_str_queue(debug_draw_str_request& request, Camera& camera);

    void draw_queue(Camera camera);
    void clear_queue();

    static void destroy() {
        delete instance;
        instance = nullptr;
    }
};

#endif // DEBUG_DRAW_H
