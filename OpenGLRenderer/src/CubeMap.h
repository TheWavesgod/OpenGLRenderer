#pragma once

#include "glad/glad.h"

#include <vector>
#include <string>

class Shader;

class CubeMap
{
public:
	CubeMap(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);

	void Draw(Shader& s);

private:
	GLuint textureID;
	GLuint SkyboxVAO;

	std::vector<std::string> paths;
	
	void CreateCubeMapMesh();

	void LoadCubeMap();
};