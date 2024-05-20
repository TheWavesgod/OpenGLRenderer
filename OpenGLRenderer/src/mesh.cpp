#include "mesh.h"
#include <iostream>

mesh* mesh::GenerateTriangle()
{
	mesh* m = new mesh;
	
	m->vertices = {
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3( 0.5f, -0.5f, 0.0f),
		glm::vec3( 0.0f,  0.5f, 0.0f)
	};

	m->texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.5f, 1.0f)
	};

	m->BufferData();

	m->drawShader = 0;

	return m;
}

mesh* mesh::GenerateQuad()
{
	mesh* m = new mesh;

	m->vertices = {
		glm::vec3( 0.5f,  0.5f, 0.0f),
		glm::vec3( 0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f,  0.5f, 0.0f)
	};

	m->colors = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f)
	};

	m->indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	m->BufferData();
	m->drawShader = 0;

	return m;
}

mesh::mesh()
{

}

void mesh::Draw()
{
	glBindVertexArray(VertexArrayID);
	if (indices.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void mesh::BufferData()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	BufferAttribute(&VBOs[VERTEX_BUFFER], vertices.size(), sizeof(glm::vec3), 3, VERTEX_BUFFER, vertices.data());

	if (!colors.empty())
	{
		BufferAttribute(&VBOs[COLOUR_BUFFER], vertices.size(), sizeof(glm::vec3), 3, COLOUR_BUFFER, colors.data());
	}

	if (!texCoords.empty())
	{
		BufferAttribute(&VBOs[TEXTURE_BUFFER], vertices.size(), sizeof(glm::vec2), 2, TEXTURE_BUFFER, texCoords.data());
	}

	if (!indices.empty())
	{
		BufferIndices(&VBOs[INDEX_BUFFER], indices.size(), indices.data());
	}

}

void mesh::BufferAttribute(GLuint* VBO, unsigned int numVertex, unsigned int dataSize, int attribSize, int attribID, void* pointer)
{
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	// Transfer Data to the buffer, GL_STATIC_DRAW define the type of buffer (memory)
	glBufferData(GL_ARRAY_BUFFER, numVertex * dataSize, pointer, GL_STATIC_DRAW);

	glVertexAttribPointer(attribID, attribSize, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribID);
}

void mesh::BufferIndices(GLuint* EBO, unsigned int numIndex, void* pointer)
{
	glGenBuffers(1, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndex * sizeof(unsigned int), pointer, GL_STATIC_DRAW);
}
