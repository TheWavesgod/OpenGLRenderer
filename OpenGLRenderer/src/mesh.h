#pragma once

#include "glfw3.h"
#include "glad/glad.h"
#include "glm.hpp"

#include <vector>

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

class mesh
{
public:
	static mesh* GenerateTriangle();
	static mesh* GenerateQuad();

	mesh();

	void Draw();

	void BufferData();

protected:
	GLuint drawShader;

	GLuint VertexArrayID;
	
	GLuint VBOs[MAX_BUFFER];

	// Mesh attributes
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	void BufferAttribute(GLuint* VBO, unsigned int numVertex, unsigned int dataSize, int attribSize, int attribID, void* pointer);
	void BufferIndices(GLuint* EBO, unsigned int numIndex, void* pointer);
private:

public:
	inline GLuint DrawShader() const { return drawShader; }
	inline GLuint VertexArrayObj() const { return VertexArrayID; }
};

