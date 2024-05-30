#pragma once

#include "glfw3.h"
#include "glad/glad.h"

#include "Utilities.h"

#include <vector>


class Shader;
class Texture;

enum MeshBuffer
{
	VERTEX = 0,
	TEXCOORD,
	NORMAL,
	COLOUR,
	TANGENT,

	MAXBUFFER
};

class Mesh
{
public:
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();
	static Mesh* GenerateCube();
	 
	Mesh();
	Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures);

	void Draw(Shader& shader);

protected:
	// Mesh attributes
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures; 

	//std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;

	void SetupMesh();
	void UploadAttribute();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

