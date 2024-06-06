#include "mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <iostream>

Mesh* Mesh::GenerateTriangle()
{
	Mesh* m = new Mesh;
	
	m->vertices = {
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3( 0.5f, -0.5f, 0.0f), 
		glm::vec3( 0.0f,  0.5f, 0.0f)
	};

	m->colors = {
		glm::vec4(0.0f),
		glm::vec4(0.0f),
		glm::vec4(0.0f)
	};

	m->texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.5f, 1.0f)
	};

	m->SetupMesh();

	return m;
}

Mesh* Mesh::GenerateQuad()
{
	Mesh* m = new Mesh;

	m->vertices = {
		glm::vec3( 0.5f,  0.5f, 0.0f), 
		glm::vec3( 0.5f, -0.5f, 0.0f), 
		glm::vec3(-0.5f, -0.5f, 0.0f), 
		glm::vec3(-0.5f,  0.5f, 0.0f)
	};

	m->colors = {
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
	};

	m->texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
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
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f), 
		glm::vec3( 0.5f, -0.5f, -0.5f), 
		glm::vec3( 0.5f,  0.5f, -0.5f), 
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f), 

		glm::vec3(-0.5f, -0.5f,  0.5f), 
		glm::vec3( 0.5f, -0.5f,  0.5f), 
		glm::vec3( 0.5f,  0.5f,  0.5f), 
		glm::vec3( 0.5f,  0.5f,  0.5f), 
		glm::vec3(-0.5f,  0.5f,  0.5f), 
		glm::vec3(-0.5f, -0.5f,  0.5f), 

		glm::vec3(-0.5f,  0.5f,  0.5f), 
		glm::vec3(-0.5f,  0.5f, -0.5f), 
		glm::vec3(-0.5f, -0.5f, -0.5f), 
		glm::vec3(-0.5f, -0.5f, -0.5f), 
		glm::vec3(-0.5f, -0.5f,  0.5f), 
		glm::vec3(-0.5f,  0.5f,  0.5f),

		glm::vec3( 0.5f,  0.5f,  0.5f), 
		glm::vec3( 0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f), 
		glm::vec3( 0.5f, -0.5f, -0.5f), 
		glm::vec3( 0.5f,  0.5f,  0.5f), 
		glm::vec3( 0.5f, -0.5f,  0.5f), 

		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f, -0.5f), 
		glm::vec3( 0.5f, -0.5f,  0.5f),
		glm::vec3( 0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f), 

		glm::vec3(-0.5f,  0.5f, -0.5f), 
		glm::vec3( 0.5f,  0.5f,  0.5f), 
		glm::vec3( 0.5f,  0.5f, -0.5f), 
		glm::vec3( 0.5f,  0.5f,  0.5f), 
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f)
	};

	m->colors =
	{
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f), 
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
		
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),

		glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f),

		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),

		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),

		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
	};

	m->texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),

		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),

		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),

		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f)
	};

	m->SetupMesh();

	return m;
}

Mesh* Mesh::GenerateFloor()
{
	Mesh* m = new Mesh();
	
	m->vertices = {
		glm::vec3(-10.0f, 0.0f, -10.0f),
		glm::vec3( 10.0f, 0.0f, -10.0f),
		glm::vec3( 10.0f, 0.0f,  10.0f),
		glm::vec3(-10.0f, 0.0f,  10.0f),
	};

	m->normals = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	m->texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	m->textures.emplace_back(Texture("../Resources/Textures/wood.png"));

	m->indices = {
		0, 3, 2, // first triangle
		2, 1, 0  // second triangle
	};

	m->SetupMesh();

	return m;
}

Mesh::Mesh()
{

}

Mesh::Mesh(std::vector<unsigned int>&& indices, std::vector<Texture>&& textures)
{
	this->indices = indices;
	this->textures = textures;
	
	SetupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(MAXBUFFER, VBOs);
}

void Mesh::Draw(Shader& shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);			// activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetUniformInt("material." + name + number, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].TextureID());
	}


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
	glBindVertexArray(VAO);

	size_t numVertices = vertices.size();
	UploadAttribute(&VBOs[VERTEX], numVertices, sizeof(glm::vec3), 3, VERTEX, vertices.data());

	if (!colors.empty())
	{
		UploadAttribute(&VBOs[COLOUR], numVertices, sizeof(glm::vec4), 4, COLOUR, colors.data());
	}
	if (!texCoords.empty())
	{
		UploadAttribute(&VBOs[TEXCOORD], numVertices, sizeof(glm::vec2), 2, TEXCOORD, texCoords.data());
	}
	if (!normals.empty())
	{
		UploadAttribute(&VBOs[NORMAL], numVertices, sizeof(glm::vec3), 3, NORMAL, normals.data());
	}
	if (!tangents.empty())
	{
		UploadAttribute(&VBOs[TANGENT], numVertices, sizeof(glm::vec3), 3, TANGENT, tangents.data());
	}
	if (!biTangents.empty())
	{
		UploadAttribute(&VBOs[BiTANGENT], numVertices, sizeof(glm::vec3), 3, BiTANGENT, biTangents.data());
	}
	if (!indices.empty())
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::UploadAttribute(GLuint* buffer, int numElements, int dataSize, int attributeSize, int attributeID, void* dataPtr)
{
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ARRAY_BUFFER, numElements * dataSize, dataPtr, GL_STATIC_DRAW);

	glEnableVertexAttribArray(attributeID);
	glVertexAttribPointer(attributeID, attributeSize, GL_FLOAT, GL_FALSE, 0, 0);
}
