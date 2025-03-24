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

class debug_draw {
private:
    static debug_draw* instance;

public:
    std::shared_ptr<line_geometry> line;
    std::shared_ptr<line_geometry> ones_line;
    std::shared_ptr<line_geometry> basis;

private:
    std::shared_ptr<Shader> debug_shader;

    std::vector<debug_draw_request> debug_draw_queue;

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

    void draw_queue(Camera camera);
    void clear_queue();

    void draw_immediate(Camera camera, debug_draw_request request);

    static void destroy() {
        delete instance;
        instance = nullptr;
    }
};

#endif // DEBUG_DRAW_H
