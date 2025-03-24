#include "debug_draw.h"

// initialize the singleton instance
debug_draw* debug_draw::instance = nullptr;

debug_draw::debug_draw() {
    debug_shader = std::shared_ptr<Shader>(new Shader("glacier/vertex.glsl", "glacier/debug_fragment.glsl"));

    std::vector<vertex> vertices;
    vertices.push_back(vertex({0,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    vertices.push_back(vertex({1,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    line = std::shared_ptr<line_geometry>(new line_geometry(vertices));

    
    std::vector<vertex> ones_vertices;
    ones_vertices.push_back(vertex({0,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    ones_vertices.push_back(vertex({1,1,1}, {0,0,0}, {0,0}, {0,0,0}));
    ones_line = std::shared_ptr<line_geometry>(new line_geometry(ones_vertices));

    std::vector<vertex> basis_vertices;
    basis_vertices.push_back(vertex({0,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    basis_vertices.push_back(vertex({1,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    basis_vertices.push_back(vertex({0,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    basis_vertices.push_back(vertex({0,1,0}, {0,0,0}, {0,0}, {0,0,0}));
    basis_vertices.push_back(vertex({0,0,0}, {0,0,0}, {0,0}, {0,0,0}));
    basis_vertices.push_back(vertex({0,0,1}, {0,0,0}, {0,0}, {0,0,0}));
    basis = std::shared_ptr<line_geometry>(new line_geometry(basis_vertices));
}

void debug_draw::draw_line(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    debug_draw_request ddr;
    ddr.color = color;
    ddr.lines = line;
    ddr.world = glm::mat4(1.0);

    debug_draw_queue.push_back(ddr);
}

void debug_draw::draw_basis(glm::mat4 world, glm::vec3 color) {
    debug_draw_request ddr;
    ddr.color = color;
    ddr.lines = basis;
    ddr.world = world;

    debug_draw_queue.push_back(ddr);
}

void debug_draw::draw_queue(Camera camera)
{
    for (auto& request : debug_draw_queue)
    {
        debug_shader->bind();
        debug_shader->set_uniform("model", request.world);
        debug_shader->set_uniform("view", camera.view);
        debug_shader->set_uniform("projection", camera.projection);
        debug_shader->set_uniform("debug_color", request.color);

        debug_shader->set_imgui_uniforms();

        request.lines->draw();
    }
}

void debug_draw::clear_queue()
{
    debug_draw_queue.clear();
}

void debug_draw::draw_immediate(Camera camera, debug_draw_request request)
{
    debug_shader->bind();
    debug_shader->set_uniform("model", request.world);
    debug_shader->set_uniform("view", camera.view);
    debug_shader->set_uniform("projection", camera.projection);
    debug_shader->set_uniform("debug_color", request.color);

    debug_shader->set_imgui_uniforms();

    request.lines->draw();
}