#include "mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <iostream>

Mesh* Mesh::GenerateTriangle()
{
	Mesh* m = new Mesh;
	
	m->vertices = {
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.5f, 1.0f)}
	};

	m->SetupMesh();

	return m;
}

Mesh* Mesh::GenerateQuad()
{
	Mesh* m = new Mesh;

	m->vertices = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
	};

	m->indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	m->SetupMesh();

	return m;
}

Mesh* Mesh::GenerateCube()
{
	Mesh* m = new Mesh;

	m->vertices = {
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
	};

	m->indices = {
		// front
		4, 5, 6,
		4, 6, 7,
		// back
		0, 3, 2,
		0, 2, 1,
		// left
		0, 7, 3,
		0, 4, 7,
		// right
		1, 2, 6,
		1, 6, 5,
		// Top
		3, 7, 6,
		3, 6, 2,
		// Bottom
		0, 1, 5,
		0, 5, 4
	};

	m->SetupMesh();

	return m;
}

Mesh::Mesh()
{

}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	glBindVertexArray(VAO);
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

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),indices.data(), GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}
