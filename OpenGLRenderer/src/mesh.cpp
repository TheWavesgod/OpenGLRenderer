#include "mesh.h"

mesh* mesh::GenerateTriangle()
{
	mesh* m = new mesh;
	
	m->vertices = {
		{-0.5f, -0.5f, 0.0f},
		{ 0.5f, -0.5f, 0.0f},
		{ 0.0f,  0.5f, 0.0f}
	};

	return m;
}

void mesh::BufferData()
{
	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);
}
