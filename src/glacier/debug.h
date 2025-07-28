#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

std::ostream& operator<<(std::ostream& out, glm::vec2 const& v)
{
    out << std::fixed << std::setprecision(2) << std::setfill('0');
    out << "[" << v.x << ",\t"
               << v.y << "]" << std::endl;
}

std::ostream& operator<<(std::ostream& out, glm::vec3 const& v)
{
    out << std::fixed << std::setprecision(2) << std::setfill('0');
    out << "[" << v.x << ",\t"
               << v.y << ",\t" 
               << v.z << "]" << std::endl;
}

std::ostream& operator<<(std::ostream& out, glm::vec4 const& v)
{
    out << std::fixed << std::setprecision(2) << std::setfill('0');
    out << "[" << v.x << ",\t"
               << v.y << ",\t" 
               << v.z << ",\t" 
               << v.w << "]" << std::endl;
}

#endif