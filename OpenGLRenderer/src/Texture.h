#pragma once

#include "glad/glad.h"
#include <string>

class Texture
{
public:
	Texture();

	Texture(const std::string& fileLoc, const std::string type = "diffuse");

	~Texture() {}

	virtual void LoadFromFile(const std::string& fileLoc);

	std::string type;
	std::string path;

protected:
	GLuint textureID;

private:

public:
	inline GLuint TextureID() const { return textureID; }
};



