#include "triangles.h"

triangle_geometry::triangle_geometry()
{

}

triangle_geometry::triangle_geometry(std::vector<vertex> vertices, std::vector<unsigned int> indices)
{
	init(vertices, indices);
}

void triangle_geometry::init(std::vector<vertex> vertices, std::vector<unsigned int> indices)
{
	vertex_count = indices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	//each vertex contains 
	//vec3 pos
	//vec3 normal
	//vec2 texcoord
	//vec3 color

	//TODO: automate this

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void triangle_geometry::draw()
{
	glBindVertexArray(VAO); //use GL_LINES for wireframe
	glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);

	//for reference later for unindexed primitives:
	//and now draw the cube:
	//glBindVertexArray(cube_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
}

triangle_geometry::~triangle_geometry()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}