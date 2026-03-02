#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

std::ostream& operator<<(std::ostream& out, glm::vec2 const& v);
std::ostream& operator<<(std::ostream& out, glm::vec3 const& v);
std::ostream& operator<<(std::ostream& out, glm::vec4 const& v);

#define log_gl_error_status() (spdlog::info("{}: {} error status: {}", __FILE__, __LINE__, glGetError()))

//TODO: need one for std::vector<>

#endif