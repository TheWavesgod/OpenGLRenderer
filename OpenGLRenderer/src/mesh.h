#pragma once

#include "glfw3.h"
#include "glad/glad.h"

#include "Utilities.h"

#include <vector>


class Shader;
class Texture;
class Model;

enum MeshBuffer
{
	VERTEX = 0,
	COLOUR,
	TEXCOORD,
	NORMAL,
	TANGENT,
	BiTANGENT,

	MAXBUFFER
};

class Mesh
{
public:
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();
	static Mesh* GenerateCube();
	static Mesh* GenerateFloor();
	 
	Mesh();
	Mesh(std::vector<unsigned int>&& indices, std::vector<Texture>&& textures);

	~Mesh();

	void Draw(Shader& shader);

	unsigned int shaderIndex;

	friend Model;

protected:
	// Mesh attributes
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> biTangents;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures; 

	void SetupMesh();
	void UploadAttribute(GLuint* buffer, int numElements, int dataSize, int attribSize, int attributeID, void* dataPtr);

private:
	GLuint VAO;
	GLuint VBOs[MAXBUFFER];
	GLuint EBO;
};

