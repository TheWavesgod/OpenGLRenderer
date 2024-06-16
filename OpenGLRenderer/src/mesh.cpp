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

	m->normals = {
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),

		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),

		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),

		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),

		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),

		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
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

	m->GenerateTangentCoordsForArrays();

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
	m->textures[0].type = TEXTYPE_DIFFUSE;

	m->indices = {
		0, 3, 2, // first triangle
		2, 1, 0  // second triangle
	};

	m->SetupMesh();

	return m;
}

Mesh* Mesh::GenerateSphere()
{
	Mesh* m = new Mesh();

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;

	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			m->vertices.push_back(glm::vec3(xPos, yPos, zPos));
			m->texCoords.push_back(glm::vec2(xSegment, ySegment));
			m->normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				m->indices.push_back(y * (X_SEGMENTS + 1) + x);
				m->indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				m->indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				m->indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	m->GenerateTangentCoordsForElements(4);

	m->SetupMesh();
	m->drawMode = GL_TRIANGLE_STRIP;

	return m;
}

Mesh::Mesh()
{
	drawMode = GL_TRIANGLES;
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

void Mesh::GenerateTangentCoordsForArrays()
{
	if (!indices.empty() || vertices.empty() || texCoords.empty()) return;

	tangents.resize(vertices.size(), glm::vec3());
	biTangents.resize(vertices.size(), glm::vec3());

	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		// Positions
		glm::vec3 pos1 = vertices[i];
		glm::vec3 pos2 = vertices[i + 1];
		glm::vec3 pos3 = vertices[i + 2];

		// Texture coordinates
		glm::vec2 uv1 = texCoords[i];
		glm::vec2 uv2 = texCoords[i + 1];
		glm::vec2 uv3 = texCoords[i + 2];

		// calculate tangent/bitangent vectors of this triangle
		glm::vec3 tangent, bitangent;

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		for (size_t j = 0; j < 3; ++j)
		{
			tangents[i + j] = tangent;
			biTangents[i + j] = bitangent;
		}
	}
}

void Mesh::GenerateTangentCoordsForElements(unsigned int faceType)
{
	if (indices.empty() || vertices.empty() || texCoords.empty()) return;

	tangents.resize(vertices.size(), glm::vec3(0.0f));
	biTangents.resize(vertices.size(), glm::vec3(0.0f));

	for (size_t i = 0; i < indices.size(); i += faceType)
	{
		glm::vec3 pos1 = vertices[indices[i]];
		glm::vec3 pos2 = vertices[indices[i + 1]];
		glm::vec3 pos3 = vertices[indices[i + 2]];

		glm::vec2 uv1 = texCoords[indices[i]];
		glm::vec2 uv2 = texCoords[indices[i + 1]];
		glm::vec2 uv3 = texCoords[indices[i + 2]];

		glm::vec3 tangent, bitangent;

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		for (size_t j = 0; j < faceType; ++j)
		{
			tangents[indices[i + j]] += tangent;
			biTangents[indices[i + j]] += bitangent;
		}
	}

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		tangents[i] = glm::normalize(tangents[i]);
		biTangents[i] = glm::normalize(biTangents[i]);
	}
}

void Mesh::BindTextureSamplerBeforedraw(Shader& shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);			// activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].GetTypeName();
		if (name == "diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "specular")
			number = std::to_string(specularNr++);

		shader.SetUniformInt("material." + name, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].TextureID());
	}
	shader.SetUniformFloat("material.shininess", 32.0f);
}

void Mesh::Draw(Shader& shader)
{
	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);			// activate proper texture unit before binding
		// retrieve texture number 
		std::string name = textures[i].GetTypeName();  // TODO: Why string can't return string&
		shader.SetUniformInt("material." + name, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].TextureID());
	}
	shader.SetUniformFloat("material.shininess", 32.0f);
	shader.SetUniformFloat("heightScale", 0.1f);
	shader.SetUniformInt("irradianceMap", 6);
	shader.SetUniformInt("prefilterMap", 7);
	shader.SetUniformInt("brdfLUT", 8);

	glBindVertexArray(VAO);
	if (indices.empty())
	{
		glDrawArrays(drawMode, 0, vertices.size());
	}
	else
	{
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Mesh::DrawToLightDepthMap()
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


