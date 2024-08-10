#include "element.h"

element::element()
{

}

void element::update()
{
    //update world matrix
    world = glm::rotate(glm::mat4(1.0), rotation.x, glm::vec3(0,0,1));
    world = glm::rotate(world, rotation.y, glm::vec3(1,0,0));
    world = glm::rotate(world, rotation.z, glm::vec3(0,1,0));
    world = glm::translate(world, position);
}

void element::draw(glm::mat4x4 parent_world, Camera& camera)
{

}