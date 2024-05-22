#pragma once

#include "glfw3.h"
#include "glad/glad.h"

#include "Utilities.h"

#include <vector>


class Shader;
class Texture;

// A handy enumerator, to determine which member of the bufferObject array
//holds which data
enum MeshBuffer {
	VERTEX_BUFFER,
	COLOUR_BUFFER,
	TEXTURE_BUFFER,
	NORMAL_BUFFER,
	TANGENT_BUFFER,

	WEIGHTVALUE_BUFFER,		//new this year, weight values of vertices
	WEIGHTINDEX_BUFFER,		//new this year, indices of weights

	INDEX_BUFFER,

	MAX_BUFFER
};

class Mesh
{
public:
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();

	Mesh();



	void Draw(Shader& shader);

	void BufferData();

protected:
	GLuint drawShader;

	GLuint VertexArrayID;
	
	GLuint VBOs[MAX_BUFFER];

	// Mesh attributes
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures; 

	void BufferAttribute(GLuint* VBO, unsigned int numVertex, unsigned int dataSize, int attribSize, int attribID, void* pointer);
	void BufferIndices(GLuint* EBO, unsigned int numIndex, void* pointer);
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

public:
	inline GLuint DrawShader() const { return drawShader; }
	inline GLuint VertexArrayObj() const { return VertexArrayID; }
};

