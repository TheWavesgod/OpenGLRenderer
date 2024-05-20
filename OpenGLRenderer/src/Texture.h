#pragma once

#include "glad/glad.h"
#include <string>

class texture
{
public:
	texture(const std::string& fileLoc);

protected:
	GLuint textureID;

private:

public:
	inline GLuint TextureID() const { return textureID; }
};

