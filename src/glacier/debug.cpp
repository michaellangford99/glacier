#include "debug.h"

std::ostream& operator<<(std::ostream& out, glm::vec2 const& v)
{
    out << std::fixed << std::setprecision(2) << std::setfill('0');
    out << "[" << v.x << ",\t"
               << v.y << "]";

    return out;
}

std::ostream& operator<<(std::ostream& out, glm::vec3 const& v)
{
    out << std::fixed << std::setprecision(2) << std::setfill('0');
    out << "[" << v.x << ",\t"
               << v.y << ",\t" 
               << v.z << "]";

    return out;
}

std::ostream& operator<<(std::ostream& out, glm::vec4 const& v)
{
    out << std::fixed << std::setprecision(2) << std::setfill('0');
    out << "[" << v.x << ",\t"
               << v.y << ",\t" 
               << v.z << ",\t" 
               << v.w << "]";

    return out;
}