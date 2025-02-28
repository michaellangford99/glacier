#include "line.h"

line_geometry::line_geometry(std::vector<vertex> _vertices) : vertices(_vertices)
{
	vertex_count = _vertices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

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

void line_geometry::draw()
{
	glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, vertex_count);
    glBindVertexArray(0);
}

line_geometry::~line_geometry()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}